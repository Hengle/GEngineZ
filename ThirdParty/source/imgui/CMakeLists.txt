project(imgui)

cmake_minimum_required(VERSION 3.0)

set(IMGUI_Header imgui.h imgui_internal.h imconfig.h imstb_rectpack.h imstb_textedit.h imstb_truetype.h)
set(IMGUI_Src imgui.cpp imgui_demo.cpp imgui_draw.cpp imgui_widgets.cpp)

add_library(imgui ${IMGUI_Header} ${IMGUI_Src})

install(FILES ${IMGUI_Header} DESTINATION include/imgui)
install(TARGETS imgui LIBRARY DESTINATION lib)



