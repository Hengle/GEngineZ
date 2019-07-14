
project(GameZ)

cmake_minimum_required(VERSION 3.0)

# ========== COMPILE OPTIONS ==========
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/Binary)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/Binary)

add_definitions(-DCMAKE_HELLDDO)

add_definitions(-DCMAKE_HELLO)



# ========== Library Engine ==========
include_directories(Engine)

set(Engine_SRC Engine/Core/Common/Common.h Engine/Core/Common/Debug.h Engine/Core/Common/Logger.h Engine/Core/Common/Singleton.h Engine/Core/Common/Time.h Engine/Client/Logic/ClientDirector.cc Engine/Client/Logic/ClientDirector.h Engine/Client/Scene/Scene.h Engine/RHIDX12/todo Engine/Core/Logic/Director.cc Engine/Core/Logic/Director.h Engine/Core/Logic/frame.h Engine/Core/Logic/view.cc Engine/Core/Logic/view.h Engine/Core/Reflection/todo Engine/Platform/Win32/win32App.cc Engine/Platform/Win32/win32App.h Engine/Platform/Win32/Win32Window.cc Engine/Platform/Win32/Win32Window.h Engine/Core/Scheduler/Scheduler.h Engine/Core/Scheduler/Service.cc Engine/Core/Scheduler/Service.h Engine/Core/Scheduler/Worker.h Engine/RHI/RHIDevice.h Engine/RHI/RHIResource.h Engine/Shader/pbr.hlsl Engine/Core/Math/todo)

set(Engine_Core_Common_GROUP_FILES Engine/Core/Common/Common.h Engine/Core/Common/Debug.h Engine/Core/Common/Logger.h Engine/Core/Common/Singleton.h Engine/Core/Common/Time.h)
source_group(Core\\Common FILES ${Engine_Core_Common_GROUP_FILES})

set(Engine_Client_Logic_GROUP_FILES Engine/Client/Logic/ClientDirector.cc Engine/Client/Logic/ClientDirector.h)
source_group(Client\\Logic FILES ${Engine_Client_Logic_GROUP_FILES})

set(Engine_Client_Scene_GROUP_FILES Engine/Client/Scene/Scene.h)
source_group(Client\\Scene FILES ${Engine_Client_Scene_GROUP_FILES})

set(Engine_RHIDX12_GROUP_FILES Engine/RHIDX12/todo)
source_group(RHIDX12 FILES ${Engine_RHIDX12_GROUP_FILES})

set(Engine_Core_Logic_GROUP_FILES Engine/Core/Logic/Director.cc Engine/Core/Logic/Director.h Engine/Core/Logic/frame.h Engine/Core/Logic/view.cc Engine/Core/Logic/view.h)
source_group(Core\\Logic FILES ${Engine_Core_Logic_GROUP_FILES})

set(Engine_Core_Reflection_GROUP_FILES Engine/Core/Reflection/todo)
source_group(Core\\Reflection FILES ${Engine_Core_Reflection_GROUP_FILES})

set(Engine_Platform_Win32_GROUP_FILES Engine/Platform/Win32/win32App.cc Engine/Platform/Win32/win32App.h Engine/Platform/Win32/Win32Window.cc Engine/Platform/Win32/Win32Window.h)
source_group(Platform\\Win32 FILES ${Engine_Platform_Win32_GROUP_FILES})

set(Engine_Core_Scheduler_GROUP_FILES Engine/Core/Scheduler/Scheduler.h Engine/Core/Scheduler/Service.cc Engine/Core/Scheduler/Service.h Engine/Core/Scheduler/Worker.h)
source_group(Core\\Scheduler FILES ${Engine_Core_Scheduler_GROUP_FILES})

set(Engine_RHI_GROUP_FILES Engine/RHI/RHIDevice.h Engine/RHI/RHIResource.h)
source_group(RHI FILES ${Engine_RHI_GROUP_FILES})

set(Engine_Shader_GROUP_FILES Engine/Shader/pbr.hlsl)
source_group(Shader FILES ${Engine_Shader_GROUP_FILES})

set(Engine_Core_Math_GROUP_FILES Engine/Core/Math/todo)
source_group(Core\\Math FILES ${Engine_Core_Math_GROUP_FILES})


add_library(Engine STATIC ${Engine_SRC})


# ========== Executable TestSched ==========


set(TestSched_SRC Test/TestSched.cc)



add_executable(TestSched ${TestSched_SRC})
target_link_libraries(TestSched Engine)


# ========== Executable Game ==========
include_directories(Program/Game)

set(Game_SRC Program/Game/Win32/main.cc)

set(Program_Game_Win32_GROUP_FILES Program/Game/Win32/main.cc)
source_group(Win32 FILES ${Program_Game_Win32_GROUP_FILES})


add_executable(Game ${Game_SRC})
target_link_libraries(Game Engine)


# ========== Custom Target Config ==========
set(Config_SRC Config/Engine.json)

add_custom_target(Config SOURCES ${Config_SRC})
