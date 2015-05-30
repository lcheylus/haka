/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include <arpa/inet.h>

#if defined(__CYGWIN__)
#include "if_ether.h"
#else
#include <linux/if_ether.h>
#endif

#include <pcap.h>
#include <pcap/vlan.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include <haka/packet_module.h>
#include <haka/log.h>
#include <haka/error.h>
#include <haka/time.h>
#include <haka/packet.h>
#include <haka/container/list.h>
#include <haka/pcap.h>

int get_link_type_offset(int link_type)
{
	size_t size;

	switch (link_type)
	{
	case DLT_LINUX_SLL: size = sizeof(struct linux_sll_header); break;
	case DLT_EN10MB:    size = sizeof(struct ethhdr); break;
	case DLT_IPV4:
	case DLT_RAW:       size = 0; break;
	case DLT_NULL:      size = 4; break;

	default:            assert(!"unsupported link type"); return -1;
	}

	return size;
}

static int get_eth_protocol(int proto, struct vbuffer *packet_buffer, size_t *data_offset)
{
	size_t len, size;
	struct vbuffer_sub sub;

	switch (proto)
	{
	case ETH_P_8021Q:
		{
			struct vlan_tag *vt;
			size = sizeof(struct vlan_tag);
			vbuffer_sub_create(&sub, packet_buffer, *data_offset, size);
			vt = (struct vlan_tag *)vbuffer_sub_flatten(&sub, &len);

			if (vt == NULL || len < size) {
				return -1;
			}

			*data_offset += size;

			return get_eth_protocol(ntohs(vt->vlan_tci), packet_buffer, data_offset);
		}
	default:
		return proto;
	}
}

int get_protocol(int link_type, struct vbuffer *packet_buffer, size_t *data_offset)
{
	size_t len, size;
	struct vbuffer_sub sub;
	const uint8* data = NULL;

	size = get_link_type_offset(link_type);
	*data_offset = size;

	if (size > 0) {
		vbuffer_sub_create(&sub, packet_buffer, 0, size);
		data = vbuffer_sub_flatten(&sub, &len);

		if (data == NULL || len < *data_offset) {
			return -1;
		}

		assert(packet_buffer);
	}

	switch (link_type)
	{
	case DLT_LINUX_SLL:
		{
			struct linux_sll_header *eh = (struct linux_sll_header *)data;
			if (eh) {
				return get_eth_protocol(ntohs(eh->protocol), packet_buffer, data_offset);
			}
			else {
				return 0;
			}
		}
		break;

	case DLT_EN10MB:
		{
			struct ethhdr *eh = (struct ethhdr *)data;
			if (eh) {
				return get_eth_protocol(ntohs(eh->h_proto), packet_buffer, data_offset);
			}
			else {
				return 0;
			}
		}
		break;

	case DLT_IPV4:
	case DLT_RAW:
		return ETH_P_IP;

	case DLT_NULL:
		*data_offset = 4;
		if (*(uint32 *)data == PF_INET) {
			return ETH_P_IP;
		}
		else {
			return -1;
		}
		break;

	default:
		assert(!"unsupported link type");
		return -1;
	}
}
