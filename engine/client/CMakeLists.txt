project(core)

macro(add_source_dir name)
    file(GLOB SRC_FILE ${name}/*.h ${name}/*.cc)
    list(APPEND ALL_SRC ${SRC_FILE})
	string(REPLACE "/" "\\" groupname ${name})
    source_group(${groupname} FILES ${SRC_FILE})
endmacro()

add_source_dir(core)
add_source_dir(object)
add_source_dir(scene)
add_source_dir(startup)
add_source_dir(startup/win32)

add_executable(client ${ALL_SRC})
target_link_libraries(client z_core)
