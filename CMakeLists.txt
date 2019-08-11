
project(GameZ)

cmake_minimum_required(VERSION 3.0)

# ========== COMPILE OPTIONS ==========
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_BUILD_TYPE RelWithDebInfo)

set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/Binary)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/Binary)

include_directories(ThirdParty/include)
link_directories(ThirdParty/lib)

add_definitions(-DHLSLCC_DYNLIB)

add_definitions(-DCOMPRESS_MESH_FILE)



# ========== Library Engine ==========
include_directories(Engine)

set(Engine_SRC Engine/Core/Common/Define.h Engine/Core/Common/Logger.h Engine/Core/Common/Noncopyable.h Engine/Core/Common/RefCountPtr.h Engine/Core/Common/Singleton.h Engine/Core/Common/Time.h Engine/Core/Object/IObject.h Engine/Client/Entity/IComponent.h Engine/Client/Entity/IEntity.h Engine/Core/CoreHeader.h Engine/Core/Platform/OSHeader.h Engine/Util/Image/Image.cc Engine/Util/Image/Image.h Engine/Render/Pipeline/ForwardPipeline.h Engine/Render/Pipeline/Stage.h Engine/RHIDX12/DX12Buffer.cc Engine/RHIDX12/DX12Buffer.h Engine/RHIDX12/DX12Const.h Engine/RHIDX12/DX12Device.cc Engine/RHIDX12/DX12Device.h Engine/RHIDX12/DX12Executor.cc Engine/RHIDX12/DX12Executor.h Engine/RHIDX12/DX12Header.h Engine/RHIDX12/DX12PipelineState.cc Engine/RHIDX12/DX12PipelineState.h Engine/RHIDX12/DX12Resource.cc Engine/RHIDX12/DX12Resource.h Engine/RHIDX12/DX12Shader.cc Engine/RHIDX12/DX12Shader.h Engine/RHIDX12/DX12Texture.cc Engine/RHIDX12/DX12Texture.h Engine/RHIDX12/DX12Util.h Engine/RHIDX12/DX12View.cc Engine/RHIDX12/DX12View.h Engine/RHIDX12/DX12Viewport.cc Engine/RHIDX12/DX12Viewport.h Engine/Util/Mesh/MeshGenerator.cc Engine/Util/Mesh/MeshGenerator.h Engine/Util/Mesh/ZMeshLoader.h Engine/Core/Platform/Win32/Windows.h Engine/RHIDX12/DX12/d3dx12.h Engine/Client/Component/Model.h Engine/Client/Component/Primitive.h Engine/Render/Material.cc Engine/Render/Material.h Engine/Render/RenderConst.h Engine/Render/Renderer.cc Engine/Render/Renderer.h Engine/Render/RenderItem.h Engine/Render/RenderOption.h Engine/Render/RenderScene.h Engine/Render/SubMesh.cc Engine/Render/SubMesh.h Engine/Core/Thread/todo Engine/Client/Main/App.cc Engine/Client/Main/App.h Engine/Client/Main/Director.cc Engine/Client/Main/Director.h Engine/Client/Main/Input.cc Engine/Client/Main/Input.h Engine/Core/Scheduler/Scheduler.h Engine/Core/Scheduler/Service.cc Engine/Core/Scheduler/Service.h Engine/Core/Scheduler/Worker.h Engine/RHI/RHIConst.h Engine/RHI/RHIDevice.cc Engine/RHI/RHIDevice.h Engine/RHI/RHIResource.h Engine/RHI/RHIUtil.h Engine/Startup/Win32/Win32App.cc Engine/Startup/Win32/Win32App.h Engine/Startup/Win32/Win32Input.h Engine/Startup/Win32/Win32Window.cc Engine/Startup/Win32/Win32Window.h Engine/Core/FileSystem/Directory.h Engine/Core/FileSystem/File.cc Engine/Core/FileSystem/File.h Engine/Core/Math/Camera.h Engine/Core/Math/Constant.h Engine/Core/Math/Function.h Engine/Core/Math/Matrix.h Engine/Core/Math/Vector.h Engine/Client/Scene/Camera.cc Engine/Client/Scene/Camera.h Engine/Client/Scene/CameraController.h Engine/Client/Scene/Scene.cc Engine/Client/Scene/Scene.h Engine/Client/Scene/WorldEnv.cc Engine/Client/Scene/WorldEnv.h)

set(Engine_Core_Common_GROUP_FILES Engine/Core/Common/Define.h Engine/Core/Common/Logger.h Engine/Core/Common/Noncopyable.h Engine/Core/Common/RefCountPtr.h Engine/Core/Common/Singleton.h Engine/Core/Common/Time.h)
source_group(Core\\Common FILES ${Engine_Core_Common_GROUP_FILES})

set(Engine_Core_Object_GROUP_FILES Engine/Core/Object/IObject.h)
source_group(Core\\Object FILES ${Engine_Core_Object_GROUP_FILES})

set(Engine_Client_Entity_GROUP_FILES Engine/Client/Entity/IComponent.h Engine/Client/Entity/IEntity.h)
source_group(Client\\Entity FILES ${Engine_Client_Entity_GROUP_FILES})

set(Engine_Core_GROUP_FILES Engine/Core/CoreHeader.h)
source_group(Core FILES ${Engine_Core_GROUP_FILES})

set(Engine_Core_Platform_GROUP_FILES Engine/Core/Platform/OSHeader.h)
source_group(Core\\Platform FILES ${Engine_Core_Platform_GROUP_FILES})

set(Engine_Util_Image_GROUP_FILES Engine/Util/Image/Image.cc Engine/Util/Image/Image.h)
source_group(Util\\Image FILES ${Engine_Util_Image_GROUP_FILES})

set(Engine_Render_Pipeline_GROUP_FILES Engine/Render/Pipeline/ForwardPipeline.h Engine/Render/Pipeline/Stage.h)
source_group(Render\\Pipeline FILES ${Engine_Render_Pipeline_GROUP_FILES})

set(Engine_RHIDX12_GROUP_FILES Engine/RHIDX12/DX12Buffer.cc Engine/RHIDX12/DX12Buffer.h Engine/RHIDX12/DX12Const.h Engine/RHIDX12/DX12Device.cc Engine/RHIDX12/DX12Device.h Engine/RHIDX12/DX12Executor.cc Engine/RHIDX12/DX12Executor.h Engine/RHIDX12/DX12Header.h Engine/RHIDX12/DX12PipelineState.cc Engine/RHIDX12/DX12PipelineState.h Engine/RHIDX12/DX12Resource.cc Engine/RHIDX12/DX12Resource.h Engine/RHIDX12/DX12Shader.cc Engine/RHIDX12/DX12Shader.h Engine/RHIDX12/DX12Texture.cc Engine/RHIDX12/DX12Texture.h Engine/RHIDX12/DX12Util.h Engine/RHIDX12/DX12View.cc Engine/RHIDX12/DX12View.h Engine/RHIDX12/DX12Viewport.cc Engine/RHIDX12/DX12Viewport.h)
source_group(RHIDX12 FILES ${Engine_RHIDX12_GROUP_FILES})

set(Engine_Util_Mesh_GROUP_FILES Engine/Util/Mesh/MeshGenerator.cc Engine/Util/Mesh/MeshGenerator.h Engine/Util/Mesh/ZMeshLoader.h)
source_group(Util\\Mesh FILES ${Engine_Util_Mesh_GROUP_FILES})

set(Engine_Core_Platform_Win32_GROUP_FILES Engine/Core/Platform/Win32/Windows.h)
source_group(Core\\Platform\\Win32 FILES ${Engine_Core_Platform_Win32_GROUP_FILES})

set(Engine_RHIDX12_DX12_GROUP_FILES Engine/RHIDX12/DX12/d3dx12.h)
source_group(RHIDX12\\DX12 FILES ${Engine_RHIDX12_DX12_GROUP_FILES})

set(Engine_Client_Component_GROUP_FILES Engine/Client/Component/Model.h Engine/Client/Component/Primitive.h)
source_group(Client\\Component FILES ${Engine_Client_Component_GROUP_FILES})

set(Engine_Render_GROUP_FILES Engine/Render/Material.cc Engine/Render/Material.h Engine/Render/RenderConst.h Engine/Render/Renderer.cc Engine/Render/Renderer.h Engine/Render/RenderItem.h Engine/Render/RenderOption.h Engine/Render/RenderScene.h Engine/Render/SubMesh.cc Engine/Render/SubMesh.h)
source_group(Render FILES ${Engine_Render_GROUP_FILES})

set(Engine_Core_Thread_GROUP_FILES Engine/Core/Thread/todo)
source_group(Core\\Thread FILES ${Engine_Core_Thread_GROUP_FILES})

set(Engine_Client_Main_GROUP_FILES Engine/Client/Main/App.cc Engine/Client/Main/App.h Engine/Client/Main/Director.cc Engine/Client/Main/Director.h Engine/Client/Main/Input.cc Engine/Client/Main/Input.h)
source_group(Client\\Main FILES ${Engine_Client_Main_GROUP_FILES})

set(Engine_Core_Scheduler_GROUP_FILES Engine/Core/Scheduler/Scheduler.h Engine/Core/Scheduler/Service.cc Engine/Core/Scheduler/Service.h Engine/Core/Scheduler/Worker.h)
source_group(Core\\Scheduler FILES ${Engine_Core_Scheduler_GROUP_FILES})

set(Engine_RHI_GROUP_FILES Engine/RHI/RHIConst.h Engine/RHI/RHIDevice.cc Engine/RHI/RHIDevice.h Engine/RHI/RHIResource.h Engine/RHI/RHIUtil.h)
source_group(RHI FILES ${Engine_RHI_GROUP_FILES})

set(Engine_Startup_Win32_GROUP_FILES Engine/Startup/Win32/Win32App.cc Engine/Startup/Win32/Win32App.h Engine/Startup/Win32/Win32Input.h Engine/Startup/Win32/Win32Window.cc Engine/Startup/Win32/Win32Window.h)
source_group(Startup\\Win32 FILES ${Engine_Startup_Win32_GROUP_FILES})

set(Engine_Core_FileSystem_GROUP_FILES Engine/Core/FileSystem/Directory.h Engine/Core/FileSystem/File.cc Engine/Core/FileSystem/File.h)
source_group(Core\\FileSystem FILES ${Engine_Core_FileSystem_GROUP_FILES})

set(Engine_Core_Math_GROUP_FILES Engine/Core/Math/Camera.h Engine/Core/Math/Constant.h Engine/Core/Math/Function.h Engine/Core/Math/Matrix.h Engine/Core/Math/Vector.h)
source_group(Core\\Math FILES ${Engine_Core_Math_GROUP_FILES})

set(Engine_Client_Scene_GROUP_FILES Engine/Client/Scene/Camera.cc Engine/Client/Scene/Camera.h Engine/Client/Scene/CameraController.h Engine/Client/Scene/Scene.cc Engine/Client/Scene/Scene.h Engine/Client/Scene/WorldEnv.cc Engine/Client/Scene/WorldEnv.h)
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
target_link_libraries(Game Engine zlib)


# ========== Executable SLConverter ==========
include_directories(Program/SLConverter)

set(SLConverter_SRC Program/SLConverter/main.cc)



add_executable(SLConverter ${SLConverter_SRC})
target_link_libraries(SLConverter Engine hlslcc)


# ========== Executable MeshConverter ==========
include_directories(Program/MeshConverter)

set(MeshConverter_SRC Program/MeshConverter/main.cc)



add_executable(MeshConverter ${MeshConverter_SRC})
target_link_libraries(MeshConverter Engine assimp-vc142-mt zlib)


# ========== Custom Target Config ==========
set(Config_SRC Config/Engine.xml)

add_custom_target(Config SOURCES ${Config_SRC})
