# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

find_package(DL REQUIRED)
find_package(Editline REQUIRED)

include_directories(.)
include_directories(${INIPARSER_INCLUDE_DIR})
include_directories(${EDITLINE_INCLUDE_DIR})

# Haka library shared between runtime and modules

swig_process(libhakaswig lua
	lua/hakainit.i
	lua/haka.i
	lua/swig.i
	luadebug/luadebug.i)

lua_compile(NAME libhakalua FILES
	lua/lua/color.lua
	lua/lua/class.lua
	lua/lua/events.lua
	lua/lua/context.lua
	lua/lua/parse.lua
	lua/lua/parse_result.lua
	lua/lua/grammar.lua
	lua/lua/grammar_dg.lua
	lua/lua/state_machine.lua
	lua/lua/state.lua
	lua/lua/utils.lua
	lua/lua/dissector.lua
	lua/lua/list.lua
	lua/lua/check.lua
)
lua_install(TARGET libhakalua DESTINATION share/haka/core)

if(CYGWIN)
add_library(libhaka SHARED
	packet.c
	log.c
	log_module.c
	alert.c
	alert_module.c
	module.c
	error.c
	debug.c
	pthread_barrier.c
	thread.c
	parameters.c
	colors.c
	time.c
	timer.c
	state_machine.c
	vbuffer.c
	vbuffer_data.c
	vbuffer_stream.c
	vbuffer_sub_stream.c
	regexp_module.c
	system.c
	engine.c
	container/list.c
	container/list2.c
	container/vector.c
	lua/state.c
	lua/ref.c
	lua/lua.c
	lua/marshal.c
	luadebug/interactive.c
	luadebug/complete.c
	luadebug/debugger.c
	luadebug/utils.c
	luadebug/user.c
	luadebug/user_readline.c
	luadebug/user_remote.c
	${SWIG_libhakaswig_FILES}
	../../external/lmarshal/lmarshal.c
)
else()
add_library(libhaka SHARED
	packet.c
	log.c
	log_module.c
	alert.c
	alert_module.c
	module.c
	error.c
	debug.c
	thread.c
	parameters.c
	colors.c
	time.c
	timer.c
	state_machine.c
	vbuffer.c
	vbuffer_data.c
	vbuffer_stream.c
	vbuffer_sub_stream.c
	regexp_module.c
	system.c
	engine.c
	container/list.c
	container/list2.c
	container/vector.c
	lua/state.c
	lua/ref.c
	lua/lua.c
	lua/marshal.c
	luadebug/interactive.c
	luadebug/complete.c
	luadebug/debugger.c
	luadebug/utils.c
	luadebug/user.c
	luadebug/user_readline.c
	luadebug/user_remote.c
	${SWIG_libhakaswig_FILES}
	../../external/lmarshal/lmarshal.c
)
endif()

# Add specific flags for link on Cygwin
if(CYGWIN)
       set_target_properties(libhaka PROPERTIES LINK_FLAGS -Wl,--export-all-symbols -Wl,--enable-auto-import -Wl,--whole-archive *.o -Wl,--no-whole-archive)
endif()

set_target_properties(libhaka PROPERTIES COMPILE_DEFINITIONS _GNU_SOURCE)

set_target_properties(libhaka PROPERTIES VERSION ${HAKA_VERSION_MAJOR}.${HAKA_VERSION_MINOR}.${HAKA_VERSION_PATCH}
	SOVERSION ${HAKA_VERSION_MAJOR})

target_link_libraries(libhaka LINK_PRIVATE ${DL_LIBRARIES})
target_link_libraries(libhaka LINK_PRIVATE ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(libhaka LINK_PRIVATE ${INIPARSER_LIBRARY})
target_link_libraries(libhaka LINK_PRIVATE rt)
target_link_libraries(libhaka LINK_PRIVATE m)
target_link_libraries(libhaka LINK_PRIVATE ${EDITLINE_LIBRARY})

target_link_libraries(libhaka LINK_PRIVATE  ${LUA_LIBRARIES})
add_dependencies(libhaka ${LUA_DEPENDENCY})
include_directories(${LUA_INCLUDE_DIR})
link_directories(${LUA_LIBRARY_DIR})

set_target_properties(libhaka PROPERTIES OUTPUT_NAME haka)

# Install
if(CYGWIN)
	install(TARGETS libhaka DESTINATION lib)
	install(FILES ${CMAKE_CURRENT_BINARY_DIR}/cyghaka-0.dll
		PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ GROUP_EXECUTE GROUP_WRITE GROUP_READ WORLD_EXECUTE WORLD_WRITE WORLD_READ
		DESTINATION bin)
else()
	install(TARGETS libhaka LIBRARY DESTINATION lib)
endif()

add_subdirectory(test)
