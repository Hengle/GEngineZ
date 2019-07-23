
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

set(Engine_SRC Engine/Core/Common/Define.h Engine/Core/Common/Logger.h Engine/Core/Common/Noncopyable.h Engine/Core/Common/RefCountPtr.h Engine/Core/Common/Singleton.h Engine/Core/Common/Time.h Engine/Render/RenderConst.h Engine/Render/RenderOption.h Engine/Render/RenderScene.cc Engine/Render/RenderScene.h Engine/Render/RenderUnit.h Engine/Core/File/todo Engine/Client/Entity/IEntity.h Engine/Core/CoreHeader.h Engine/Core/Platform/OSHeader.h Engine/Render/Pipeline/Stage.h Engine/Core/Platform/Win32/Windows.h Engine/Platform/Win32/Win32App.cc Engine/Platform/Win32/Win32App.h Engine/Platform/Win32/Win32Window.cc Engine/Platform/Win32/Win32Window.h Engine/Core/Thread/todo Engine/RHIDX12/DX12Buffer.cc Engine/RHIDX12/DX12Buffer.h Engine/RHIDX12/DX12Command.cc Engine/RHIDX12/DX12Command.h Engine/RHIDX12/DX12Const.h Engine/RHIDX12/DX12Descriptor.cc Engine/RHIDX12/DX12Descriptor.h Engine/RHIDX12/DX12Device.cc Engine/RHIDX12/DX12Device.h Engine/RHIDX12/DX12Header.h Engine/RHIDX12/DX12Helper.h Engine/RHIDX12/DX12Resource.cc Engine/RHIDX12/DX12Resource.h Engine/RHIDX12/DX12Texture.h Engine/RHIDX12/DX12Viewport.cc Engine/RHIDX12/DX12Viewport.h Engine/Core/Scheduler/Scheduler.h Engine/Core/Scheduler/Service.cc Engine/Core/Scheduler/Service.h Engine/Core/Scheduler/Worker.h Engine/RHI/RHIConst.h Engine/RHI/RHIDevice.cc Engine/RHI/RHIDevice.h Engine/RHI/RHIRenderTarget.h Engine/RHI/RHIResource.h Engine/Client/Director/AppWindow.h Engine/Client/Director/Director.cc Engine/Client/Director/Director.h Engine/Client/Director/Frame.h Engine/Client/Director/Viewport.cc Engine/Client/Director/Viewport.h Engine/Client/Director/ViewportMain.cc Engine/Client/Director/ViewportMain.h Engine/Render/Resource/RenderResource.h Engine/Render/Resource/Shader.h Engine/Render/Resource/Vertex.h Engine/Render/Resource/View.h Engine/Core/Object/IObject.h Engine/Core/Math/todo Engine/Client/Scene/Camera.cc Engine/Client/Scene/Camera.h Engine/Client/Scene/Scene.cc Engine/Client/Scene/Scene.h Engine/Client/Scene/WorldEnv.cc Engine/Client/Scene/WorldEnv.h)

set(Engine_Core_Common_GROUP_FILES Engine/Core/Common/Define.h Engine/Core/Common/Logger.h Engine/Core/Common/Noncopyable.h Engine/Core/Common/RefCountPtr.h Engine/Core/Common/Singleton.h Engine/Core/Common/Time.h)
source_group(Core\\Common FILES ${Engine_Core_Common_GROUP_FILES})

set(Engine_Render_GROUP_FILES Engine/Render/RenderConst.h Engine/Render/RenderOption.h Engine/Render/RenderScene.cc Engine/Render/RenderScene.h Engine/Render/RenderUnit.h)
source_group(Render FILES ${Engine_Render_GROUP_FILES})

set(Engine_Core_File_GROUP_FILES Engine/Core/File/todo)
source_group(Core\\File FILES ${Engine_Core_File_GROUP_FILES})

set(Engine_Client_Entity_GROUP_FILES Engine/Client/Entity/IEntity.h)
source_group(Client\\Entity FILES ${Engine_Client_Entity_GROUP_FILES})

set(Engine_Core_GROUP_FILES Engine/Core/CoreHeader.h)
source_group(Core FILES ${Engine_Core_GROUP_FILES})

set(Engine_Core_Platform_GROUP_FILES Engine/Core/Platform/OSHeader.h)
source_group(Core\\Platform FILES ${Engine_Core_Platform_GROUP_FILES})

set(Engine_Render_Pipeline_GROUP_FILES Engine/Render/Pipeline/Stage.h)
source_group(Render\\Pipeline FILES ${Engine_Render_Pipeline_GROUP_FILES})

set(Engine_Core_Platform_Win32_GROUP_FILES Engine/Core/Platform/Win32/Windows.h)
source_group(Core\\Platform\\Win32 FILES ${Engine_Core_Platform_Win32_GROUP_FILES})

set(Engine_Platform_Win32_GROUP_FILES Engine/Platform/Win32/Win32App.cc Engine/Platform/Win32/Win32App.h Engine/Platform/Win32/Win32Window.cc Engine/Platform/Win32/Win32Window.h)
source_group(Platform\\Win32 FILES ${Engine_Platform_Win32_GROUP_FILES})

set(Engine_Core_Thread_GROUP_FILES Engine/Core/Thread/todo)
source_group(Core\\Thread FILES ${Engine_Core_Thread_GROUP_FILES})

set(Engine_RHIDX12_GROUP_FILES Engine/RHIDX12/DX12Buffer.cc Engine/RHIDX12/DX12Buffer.h Engine/RHIDX12/DX12Command.cc Engine/RHIDX12/DX12Command.h Engine/RHIDX12/DX12Const.h Engine/RHIDX12/DX12Descriptor.cc Engine/RHIDX12/DX12Descriptor.h Engine/RHIDX12/DX12Device.cc Engine/RHIDX12/DX12Device.h Engine/RHIDX12/DX12Header.h Engine/RHIDX12/DX12Helper.h Engine/RHIDX12/DX12Resource.cc Engine/RHIDX12/DX12Resource.h Engine/RHIDX12/DX12Texture.h Engine/RHIDX12/DX12Viewport.cc Engine/RHIDX12/DX12Viewport.h)
source_group(RHIDX12 FILES ${Engine_RHIDX12_GROUP_FILES})

set(Engine_Core_Scheduler_GROUP_FILES Engine/Core/Scheduler/Scheduler.h Engine/Core/Scheduler/Service.cc Engine/Core/Scheduler/Service.h Engine/Core/Scheduler/Worker.h)
source_group(Core\\Scheduler FILES ${Engine_Core_Scheduler_GROUP_FILES})

set(Engine_RHI_GROUP_FILES Engine/RHI/RHIConst.h Engine/RHI/RHIDevice.cc Engine/RHI/RHIDevice.h Engine/RHI/RHIRenderTarget.h Engine/RHI/RHIResource.h)
source_group(RHI FILES ${Engine_RHI_GROUP_FILES})

set(Engine_Client_Director_GROUP_FILES Engine/Client/Director/AppWindow.h Engine/Client/Director/Director.cc Engine/Client/Director/Director.h Engine/Client/Director/Frame.h Engine/Client/Director/Viewport.cc Engine/Client/Director/Viewport.h Engine/Client/Director/ViewportMain.cc Engine/Client/Director/ViewportMain.h)
source_group(Client\\Director FILES ${Engine_Client_Director_GROUP_FILES})

set(Engine_Render_Resource_GROUP_FILES Engine/Render/Resource/RenderResource.h Engine/Render/Resource/Shader.h Engine/Render/Resource/Vertex.h Engine/Render/Resource/View.h)
source_group(Render\\Resource FILES ${Engine_Render_Resource_GROUP_FILES})

set(Engine_Core_Object_GROUP_FILES Engine/Core/Object/IObject.h)
source_group(Core\\Object FILES ${Engine_Core_Object_GROUP_FILES})

set(Engine_Core_Math_GROUP_FILES Engine/Core/Math/todo)
source_group(Core\\Math FILES ${Engine_Core_Math_GROUP_FILES})

set(Engine_Client_Scene_GROUP_FILES Engine/Client/Scene/Camera.cc Engine/Client/Scene/Camera.h Engine/Client/Scene/Scene.cc Engine/Client/Scene/Scene.h Engine/Client/Scene/WorldEnv.cc Engine/Client/Scene/WorldEnv.h)
source_group(Client\\Scene FILES ${Engine_Client_Scene_GROUP_FILES})


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
set(Config_SRC Config/Engine.xml)

add_custom_target(Config SOURCES ${Config_SRC})
