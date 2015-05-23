# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

# Search for libpcap include dirs and libraries

include(FindPackageHandleStandardArgs)
include(CheckCSourceCompiles)

if(NOT (CYGWIN))
	find_path(PCAP_INCLUDE_DIR NAMES pcap.h)
	find_library(PCAP_LIBRARY NAMES pcap)
else()
	find_path(PCAP_INCLUDE_DIR NAMES pcap/pcap.h)
	find_library(PCAP_LIBRARY NAMES wpcap)
endif()

find_package_handle_standard_args(PCAP REQUIRED_VARS PCAP_LIBRARY PCAP_INCLUDE_DIR)
