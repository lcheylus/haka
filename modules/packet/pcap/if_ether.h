/*
 * Ethernet headers definition
 */

#ifndef _IF_ETHER_H
#define _IF_ETHER_H

/*
 * IEEE 802.3 Ethernet magic constants.
 */

#define ETH_ALEN	6		/* Octets in one ethernet addr	 */
#define ETH_HLEN	14		/* Total octets in header.	 */
#define ETH_ZLEN	60		/* Min. octets in frame sans FCS */
#define ETH_DATA_LEN	1500		/* Max. octets in payload	 */
#define ETH_FRAME_LEN	1514		/* Max. octets in frame sans FCS */
#define ETH_FCS_LEN	4		/* Octets in the FCS		 */

/*
 * Ethernet Protocol ID's.
 */

#define ETH_P_LOOP	0x0060		/* Ethernet Loopback packet	*/
#define ETH_P_IP	0x0800		/* Internet Protocol packet	*/
#define ETH_P_ARP	0x0806		/* Address Resolution packet	*/
#define ETH_P_RARP      0x8035		/* Reverse Addr Res packet	*/
#define ETH_P_8021Q	0x8100          /* 802.1Q VLAN Extended Header  */
#define ETH_P_IPV6	0x86DD		/* IPv6 over bluebook		*/
#define ETH_P_PPP_DISC	0x8863		/* PPPoE discovery messages     */
#define ETH_P_PPP_SES	0x8864		/* PPPoE session messages	*/

#define ETH_P_802_3_MIN	0x0600		/* If the value in the ethernet type is less than this value
					 * then the frame is Ethernet II. Else it is 802.3 */

/*
 * Ethernet frame header.
 */

struct ethhdr {
	unsigned char	h_dest[ETH_ALEN];	/* destination eth addr	*/
	unsigned char	h_source[ETH_ALEN];	/* source ether addr	*/
	// __be16	h_proto;		/* packet type ID field	*/
	uint16_t	h_proto;		/* packet type ID field	*/
} __attribute__((packed));


#endif /* _IF_ETHER_H */
