# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

swig_add_module(ipv4 lua SHARED
	ipv4.i
	main.c
	ipv4.c
	ipv4-addr.c
	ipv4-network.c
	cnx.c
)
SWIG_FIX_ENTRYPOINT(ipv4 protocol)

# Add specific flags for link on Cygwin
if(CYGWIN)
       set_target_properties(ipv4 PROPERTIES LINK_FLAGS -Wl,--export-all-symbols)
endif()

include_directories(.)

INSTALL_MODULE(ipv4 protocol)

lua_compile(NAME ipv4lua FILES ipv4lua.lua)
lua_install(TARGET ipv4lua DESTINATION ${MODULE_INSTALL_PATH}/protocol)

install(DIRECTORY haka DESTINATION include FILES_MATCHING PATTERN "*.h" PATTERN "*.si")

# Tests
add_subdirectory(test)
