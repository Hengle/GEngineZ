project(core)

macro(add_source_dir name)
    file(GLOB SRC_FILE ${name}/*.h ${name}/*.cc)
    list(APPEND ALL_SRC ${SRC_FILE})
	string(REPLACE "/" "\\" groupname ${name})
    source_group(${groupname} FILES ${SRC_FILE})
endmacro()

add_source_dir(common)
add_source_dir(foundation)
add_source_dir(input)
add_source_dir(objet)
add_source_dir(reflect)
add_source_dir(sched)
add_source_dir(util)

add_library(z_core STATIC ${ALL_SRC})

