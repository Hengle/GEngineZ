project(core)

macro(add_test_case name)
	add_executable(${name} ${name}.cc)
	target_link_libraries(${name} z_core)
endmacro()

add_test_case(test_sched)

