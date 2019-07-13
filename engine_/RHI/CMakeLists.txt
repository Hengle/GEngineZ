project(device)

macro(add_source_dir name)
    file(GLOB SRC_FILE ${name}/*.h ${name}/*.cc)
    list(APPEND ALL_SRC ${SRC_FILE})
	string(REPLACE "/" "\\" groupname ${name})
    source_group(${groupname} FILES ${SRC_FILE})
endmacro()

add_source_dir(dx12)

add_executable(device ${ALL_SRC})
target_link_libraries(device z_core)
