# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

find_package(PCAP REQUIRED)
if(PCAP_FOUND)
	add_library(packet-pcap SHARED main.c pcap.c)
	set_target_properties(packet-pcap PROPERTIES OUTPUT_NAME pcap)

	include_directories(${PCAP_INCLUDE_DIR})

	# Specific path for includes on Windows with WinPCAP
	if(CYGWIN)
		include_directories("${PCAP_INCLUDE_DIR}/pcap/")
	endif()

	target_link_libraries(packet-pcap ${PCAP_LIBRARY})

	INSTALL_MODULE(packet-pcap packet)
endif()
