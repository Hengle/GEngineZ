
project(GameZ)

cmake_minimum_required(VERSION 3.0)

# ========== COMPILE OPTIONS ==========
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_BUILD_TYPE RelWithDebInfo)

set(BIN_DIR ${PROJECT_SOURCE_DIR}/Binary)

set(LIBRARY_OUTPUT_PATH ${BIN_DIR})
set(EXECUTABLE_OUTPUT_PATH ${BIN_DIR})

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BIN_DIR} )
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${BIN_DIR} )
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${BIN_DIR} )
foreach( OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES} )
    string( TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG )
    set( CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${BIN_DIR} )
    set( CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${BIN_DIR} )
    set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${BIN_DIR} )
endforeach( OUTPUTCONFIG CMAKE_CONFIGURATION_TYPES )

add_compile_options(/wd4267)
add_compile_options(/wd4267)
add_compile_options(/wd26451)
add_compile_options(/wd26444)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# ========== MSVS VERSION ==========
if( MSVC )
  # in order to prevent DLL hell, each of the DLLs have to be suffixed with the major version and msvc prefix
  # CMake 3.12 added a variable for this
  if(MSVC_TOOLSET_VERSION)
    set(MSVC_PREFIX "vc${MSVC_TOOLSET_VERSION}")
  else()
    if( MSVC70 OR MSVC71 )
      set(MSVC_PREFIX "vc70")
    elseif( MSVC80 )
      set(MSVC_PREFIX "vc80")
    elseif( MSVC90 )
      set(MSVC_PREFIX "vc90")
    elseif( MSVC10 )
      set(MSVC_PREFIX "vc100")
    elseif( MSVC11 )
      set(MSVC_PREFIX "vc110")
    elseif( MSVC12 )
      set(MSVC_PREFIX "vc120")
    elseif( MSVC_VERSION LESS 1910)
      set(MSVC_PREFIX "vc140")
    elseif( MSVC_VERSION LESS 1920)
      set(MSVC_PREFIX "vc141")
    elseif( MSVC_VERSION LESS 1930)
      set(MSVC_PREFIX "vc142")
    else()
      MESSAGE(WARNING "unknown msvc version ${MSVC_VERSION}")
      set(MSVC_PREFIX "vc150")
    endif()
  endif()
endif()

# ========== INCLUDE DIECTORY ==========
include_directories(ThirdParty/include)
link_directories(ThirdParty/lib)

add_definitions(-DHLSLCC_DYNLIB)

add_definitions(-DCOMPRESS_MESH_FILE)



# ========== Library Engine ==========
include_directories(Engine)

set(Engine_SRC Engine/Core/Common/Define.h Engine/Core/Common/Logger.h Engine/Core/Common/Noncopyable.h Engine/Core/Common/RefCountPtr.h Engine/Core/Common/Singleton.h Engine/Core/Common/Time.h Engine/Core/CoreHeader.h Engine/Render/Pipeline/BaseScreenStep.h Engine/Render/Pipeline/ForwardMainStep.h Engine/Render/Pipeline/HDRStep.h Engine/Render/Pipeline/IMGuiStep.cc Engine/Render/Pipeline/IMGuiStep.h Engine/Render/Pipeline/RenderScene.h Engine/Render/Pipeline/RenderStep.h Engine/Core/Thread/todo Engine/Startup/Win32/Win32App.cc Engine/Startup/Win32/Win32App.h Engine/Startup/Win32/Win32IMGuiImpl.cc Engine/Startup/Win32/Win32IMGuiImpl.h Engine/Startup/Win32/Win32Input.h Engine/Startup/Win32/Win32Window.cc Engine/Startup/Win32/Win32Window.h Engine/RHIDX12/DX12Buffer.cc Engine/RHIDX12/DX12Buffer.h Engine/RHIDX12/DX12Const.h Engine/RHIDX12/DX12Device.cc Engine/RHIDX12/DX12Device.h Engine/RHIDX12/DX12Executor.cc Engine/RHIDX12/DX12Executor.h Engine/RHIDX12/DX12Header.h Engine/RHIDX12/DX12PipelineState.cc Engine/RHIDX12/DX12PipelineState.h Engine/RHIDX12/DX12Resource.cc Engine/RHIDX12/DX12Resource.h Engine/RHIDX12/DX12Shader.cc Engine/RHIDX12/DX12Shader.h Engine/RHIDX12/DX12Texture.cc Engine/RHIDX12/DX12Texture.h Engine/RHIDX12/DX12Util.h Engine/RHIDX12/DX12View.cc Engine/RHIDX12/DX12View.h Engine/RHIDX12/DX12Viewport.cc Engine/RHIDX12/DX12Viewport.h Engine/Util/Image/Image.cc Engine/Util/Image/Image.h Engine/Core/Platform/Win32/Windows.h Engine/Client/Main/App.cc Engine/Client/Main/App.h Engine/Client/Main/Director.cc Engine/Client/Main/Director.h Engine/Client/Main/Input.cc Engine/Client/Main/Input.h Engine/Core/Object/IObject.h Engine/Core/Math/Camera.h Engine/Core/Math/Geometry.h Engine/Core/Math/GeometryAlg.h Engine/Core/Math/LinearAlg.h Engine/Core/Math/Matrix.h Engine/Core/Math/Number.h Engine/Core/Math/Vector.h Engine/Client/Scene/Camera.cc Engine/Client/Scene/Camera.h Engine/Client/Scene/Picker.h Engine/Client/Scene/Scene.cc Engine/Client/Scene/Scene.h Engine/Util/Mesh/MeshGenerator.cc Engine/Util/Mesh/MeshGenerator.h Engine/Util/Mesh/ZMeshLoader.h Engine/Core/Scheduler/Scheduler.h Engine/Core/Scheduler/Service.cc Engine/Core/Scheduler/Service.h Engine/Core/Scheduler/Worker.h Engine/Core/Platform/OSHeader.h Engine/Client/Editor/CameraController.h Engine/Client/Editor/EditorUI.cc Engine/Client/Editor/EditorUI.h Engine/Client/Entity/IComponent.cc Engine/Client/Entity/IComponent.h Engine/Client/Entity/IEntity.cc Engine/Client/Entity/IEntity.h Engine/Client/Entity/Transform.h Engine/RHIDX12/DX12/d3dx12.h Engine/Client/Component/EnvComp.cc Engine/Client/Component/EnvComp.h Engine/Client/Component/PrimitiveComp.cc Engine/Client/Component/PrimitiveComp.h Engine/RHI/RHIConst.h Engine/RHI/RHIDevice.cc Engine/RHI/RHIDevice.h Engine/RHI/RHIResource.h Engine/RHI/RHIUtil.h Engine/Render/Material.cc Engine/Render/Material.h Engine/Render/MaterialMgr.cc Engine/Render/Mesh.cc Engine/Render/Mesh.h Engine/Render/RenderConst.h Engine/Render/Renderer.cc Engine/Render/Renderer.h Engine/Render/RenderItem.h Engine/Render/RenderOption.h Engine/Render/RenderStage.cc Engine/Render/RenderStage.h Engine/Render/RenderTarget.h Engine/Render/SceneCollection.h Engine/Render/TexManager.h Engine/Util/Luaconf/Luaconf.h Engine/Util/Luaconf/LValue.h Engine/Core/FileSystem/Directory.h Engine/Core/FileSystem/File.cc Engine/Core/FileSystem/File.h)

set(Engine_Core_Common_GROUP_FILES Engine/Core/Common/Define.h Engine/Core/Common/Logger.h Engine/Core/Common/Noncopyable.h Engine/Core/Common/RefCountPtr.h Engine/Core/Common/Singleton.h Engine/Core/Common/Time.h)
source_group(Core\\Common FILES ${Engine_Core_Common_GROUP_FILES})

set(Engine_Core_GROUP_FILES Engine/Core/CoreHeader.h)
source_group(Core FILES ${Engine_Core_GROUP_FILES})

set(Engine_Render_Pipeline_GROUP_FILES Engine/Render/Pipeline/BaseScreenStep.h Engine/Render/Pipeline/ForwardMainStep.h Engine/Render/Pipeline/HDRStep.h Engine/Render/Pipeline/IMGuiStep.cc Engine/Render/Pipeline/IMGuiStep.h Engine/Render/Pipeline/RenderScene.h Engine/Render/Pipeline/RenderStep.h)
source_group(Render\\Pipeline FILES ${Engine_Render_Pipeline_GROUP_FILES})

set(Engine_Core_Thread_GROUP_FILES Engine/Core/Thread/todo)
source_group(Core\\Thread FILES ${Engine_Core_Thread_GROUP_FILES})

set(Engine_Startup_Win32_GROUP_FILES Engine/Startup/Win32/Win32App.cc Engine/Startup/Win32/Win32App.h Engine/Startup/Win32/Win32IMGuiImpl.cc Engine/Startup/Win32/Win32IMGuiImpl.h Engine/Startup/Win32/Win32Input.h Engine/Startup/Win32/Win32Window.cc Engine/Startup/Win32/Win32Window.h)
source_group(Startup\\Win32 FILES ${Engine_Startup_Win32_GROUP_FILES})

set(Engine_RHIDX12_GROUP_FILES Engine/RHIDX12/DX12Buffer.cc Engine/RHIDX12/DX12Buffer.h Engine/RHIDX12/DX12Const.h Engine/RHIDX12/DX12Device.cc Engine/RHIDX12/DX12Device.h Engine/RHIDX12/DX12Executor.cc Engine/RHIDX12/DX12Executor.h Engine/RHIDX12/DX12Header.h Engine/RHIDX12/DX12PipelineState.cc Engine/RHIDX12/DX12PipelineState.h Engine/RHIDX12/DX12Resource.cc Engine/RHIDX12/DX12Resource.h Engine/RHIDX12/DX12Shader.cc Engine/RHIDX12/DX12Shader.h Engine/RHIDX12/DX12Texture.cc Engine/RHIDX12/DX12Texture.h Engine/RHIDX12/DX12Util.h Engine/RHIDX12/DX12View.cc Engine/RHIDX12/DX12View.h Engine/RHIDX12/DX12Viewport.cc Engine/RHIDX12/DX12Viewport.h)
source_group(RHIDX12 FILES ${Engine_RHIDX12_GROUP_FILES})

set(Engine_Util_Image_GROUP_FILES Engine/Util/Image/Image.cc Engine/Util/Image/Image.h)
source_group(Util\\Image FILES ${Engine_Util_Image_GROUP_FILES})

set(Engine_Core_Platform_Win32_GROUP_FILES Engine/Core/Platform/Win32/Windows.h)
source_group(Core\\Platform\\Win32 FILES ${Engine_Core_Platform_Win32_GROUP_FILES})

set(Engine_Client_Main_GROUP_FILES Engine/Client/Main/App.cc Engine/Client/Main/App.h Engine/Client/Main/Director.cc Engine/Client/Main/Director.h Engine/Client/Main/Input.cc Engine/Client/Main/Input.h)
source_group(Client\\Main FILES ${Engine_Client_Main_GROUP_FILES})

set(Engine_Core_Object_GROUP_FILES Engine/Core/Object/IObject.h)
source_group(Core\\Object FILES ${Engine_Core_Object_GROUP_FILES})

set(Engine_Core_Math_GROUP_FILES Engine/Core/Math/Camera.h Engine/Core/Math/Geometry.h Engine/Core/Math/GeometryAlg.h Engine/Core/Math/LinearAlg.h Engine/Core/Math/Matrix.h Engine/Core/Math/Number.h Engine/Core/Math/Vector.h)
source_group(Core\\Math FILES ${Engine_Core_Math_GROUP_FILES})

set(Engine_Client_Scene_GROUP_FILES Engine/Client/Scene/Camera.cc Engine/Client/Scene/Camera.h Engine/Client/Scene/Picker.h Engine/Client/Scene/Scene.cc Engine/Client/Scene/Scene.h)
source_group(Client\\Scene FILES ${Engine_Client_Scene_GROUP_FILES})

set(Engine_Util_Mesh_GROUP_FILES Engine/Util/Mesh/MeshGenerator.cc Engine/Util/Mesh/MeshGenerator.h Engine/Util/Mesh/ZMeshLoader.h)
source_group(Util\\Mesh FILES ${Engine_Util_Mesh_GROUP_FILES})

set(Engine_Core_Scheduler_GROUP_FILES Engine/Core/Scheduler/Scheduler.h Engine/Core/Scheduler/Service.cc Engine/Core/Scheduler/Service.h Engine/Core/Scheduler/Worker.h)
source_group(Core\\Scheduler FILES ${Engine_Core_Scheduler_GROUP_FILES})

set(Engine_Core_Platform_GROUP_FILES Engine/Core/Platform/OSHeader.h)
source_group(Core\\Platform FILES ${Engine_Core_Platform_GROUP_FILES})

set(Engine_Client_Editor_GROUP_FILES Engine/Client/Editor/CameraController.h Engine/Client/Editor/EditorUI.cc Engine/Client/Editor/EditorUI.h)
source_group(Client\\Editor FILES ${Engine_Client_Editor_GROUP_FILES})

set(Engine_Client_Entity_GROUP_FILES Engine/Client/Entity/IComponent.cc Engine/Client/Entity/IComponent.h Engine/Client/Entity/IEntity.cc Engine/Client/Entity/IEntity.h Engine/Client/Entity/Transform.h)
source_group(Client\\Entity FILES ${Engine_Client_Entity_GROUP_FILES})

set(Engine_RHIDX12_DX12_GROUP_FILES Engine/RHIDX12/DX12/d3dx12.h)
source_group(RHIDX12\\DX12 FILES ${Engine_RHIDX12_DX12_GROUP_FILES})

set(Engine_Client_Component_GROUP_FILES Engine/Client/Component/EnvComp.cc Engine/Client/Component/EnvComp.h Engine/Client/Component/PrimitiveComp.cc Engine/Client/Component/PrimitiveComp.h)
source_group(Client\\Component FILES ${Engine_Client_Component_GROUP_FILES})

set(Engine_RHI_GROUP_FILES Engine/RHI/RHIConst.h Engine/RHI/RHIDevice.cc Engine/RHI/RHIDevice.h Engine/RHI/RHIResource.h Engine/RHI/RHIUtil.h)
source_group(RHI FILES ${Engine_RHI_GROUP_FILES})

set(Engine_Render_GROUP_FILES Engine/Render/Material.cc Engine/Render/Material.h Engine/Render/MaterialMgr.cc Engine/Render/Mesh.cc Engine/Render/Mesh.h Engine/Render/RenderConst.h Engine/Render/Renderer.cc Engine/Render/Renderer.h Engine/Render/RenderItem.h Engine/Render/RenderOption.h Engine/Render/RenderStage.cc Engine/Render/RenderStage.h Engine/Render/RenderTarget.h Engine/Render/SceneCollection.h Engine/Render/TexManager.h)
source_group(Render FILES ${Engine_Render_GROUP_FILES})

set(Engine_Util_Luaconf_GROUP_FILES Engine/Util/Luaconf/Luaconf.h Engine/Util/Luaconf/LValue.h)
source_group(Util\\Luaconf FILES ${Engine_Util_Luaconf_GROUP_FILES})

set(Engine_Core_FileSystem_GROUP_FILES Engine/Core/FileSystem/Directory.h Engine/Core/FileSystem/File.cc Engine/Core/FileSystem/File.h)
source_group(Core\\FileSystem FILES ${Engine_Core_FileSystem_GROUP_FILES})


add_library(Engine STATIC ${Engine_SRC})


# ========== Executable TestSched ==========


set(TestSched_SRC Test/TestSched.cc)



add_executable(TestSched ${TestSched_SRC})
target_link_libraries(TestSched Engine)

set_property(TARGET TestSched PROPERTY FOLDER Test)


# ========== Custom Target Shader ==========
set(Shader_SRC Shader/include/BRDF.hlsl Shader/include/CBuffer.hlsl Shader/include/Common.hlsl Shader/include/PBRCommon.hlsl Shader/EditorAxis.hlsl Shader/Empty.hlsl Shader/HDRSky.hlsl Shader/IMGui.hlsl Shader/PBR.hlsl Shader/Phong.hlsl Shader/ToneMapping.hlsl)
set(Shader_include_GROUP_FILES Shader/include/BRDF.hlsl Shader/include/CBuffer.hlsl Shader/include/Common.hlsl Shader/include/PBRCommon.hlsl)
source_group(include FILES ${Shader_include_GROUP_FILES})

add_custom_target(Shader SOURCES ${Shader_SRC})


# ========== Custom Target Content ==========
set(Content_SRC Content/Config/Engine.xml Content/Game/todo Content/Test/Scene/test.scene Content/Models/gundam.3DS Content/Models/gundam.obj Content/Models/horse.obj Content/Models/man0.obj Content/Models/nanosuit.obj Content/Engine/Texture/Sky.jpg Content/Engine/Texture/Sky2.jpg Content/Engine/Texture/UVTest.jpg Content/Test/UE/chair.material Content/Test/UE/chair.model Content/Test/UE/Color_checker.HDR Content/Test/UE/Floor.zmesh Content/Test/UE/SM_Chair_2.zmesh Content/Test/UE/SM_ColorCalibrator_4.zmesh Content/Test/UE/SM_Elegant_chair_2.zmesh Content/Test/UE/SM_MatPreviewMesh_01_7.zmesh Content/Test/UE/SM_Modern_Table_5.zmesh Content/Test/UE/T_Chair_M.tga Content/Test/UE/T_Elegant_chair_a.TGA Content/Test/UE/T_Elegant_chair_m_r_ao.TGA Content/Test/UE/T_Elegant_chair_n.TGA Content/Test/Other/gundam.model Content/Test/Other/gundam.zmesh Content/Test/Other/horse.material Content/Test/Other/horse.model Content/Test/Other/horse.zmesh Content/Test/Other/horse_b.tga Content/Test/Other/horse_m.tga Content/Test/Other/horse_n.tga Content/Test/Other/man0.material Content/Test/Other/man0.model Content/Test/Other/man0.zmesh Content/Test/Other/man0_b.tga Content/Test/Other/man0_m.tga Content/Test/Other/man0_n.tga)
set(Content_Config_GROUP_FILES Content/Config/Engine.xml)
source_group(Config FILES ${Content_Config_GROUP_FILES})

set(Content_Game_GROUP_FILES Content/Game/todo)
source_group(Game FILES ${Content_Game_GROUP_FILES})

set(Content_Test_Scene_GROUP_FILES Content/Test/Scene/test.scene)
source_group(Test\\Scene FILES ${Content_Test_Scene_GROUP_FILES})

set(Content_Models_GROUP_FILES Content/Models/gundam.3DS Content/Models/gundam.obj Content/Models/horse.obj Content/Models/man0.obj Content/Models/nanosuit.obj)
source_group(Models FILES ${Content_Models_GROUP_FILES})

set(Content_Engine_Texture_GROUP_FILES Content/Engine/Texture/Sky.jpg Content/Engine/Texture/Sky2.jpg Content/Engine/Texture/UVTest.jpg)
source_group(Engine\\Texture FILES ${Content_Engine_Texture_GROUP_FILES})

set(Content_Test_UE_GROUP_FILES Content/Test/UE/chair.material Content/Test/UE/chair.model Content/Test/UE/Color_checker.HDR Content/Test/UE/Floor.zmesh Content/Test/UE/SM_Chair_2.zmesh Content/Test/UE/SM_ColorCalibrator_4.zmesh Content/Test/UE/SM_Elegant_chair_2.zmesh Content/Test/UE/SM_MatPreviewMesh_01_7.zmesh Content/Test/UE/SM_Modern_Table_5.zmesh Content/Test/UE/T_Chair_M.tga Content/Test/UE/T_Elegant_chair_a.TGA Content/Test/UE/T_Elegant_chair_m_r_ao.TGA Content/Test/UE/T_Elegant_chair_n.TGA)
source_group(Test\\UE FILES ${Content_Test_UE_GROUP_FILES})

set(Content_Test_Other_GROUP_FILES Content/Test/Other/gundam.model Content/Test/Other/gundam.zmesh Content/Test/Other/horse.material Content/Test/Other/horse.model Content/Test/Other/horse.zmesh Content/Test/Other/horse_b.tga Content/Test/Other/horse_m.tga Content/Test/Other/horse_n.tga Content/Test/Other/man0.material Content/Test/Other/man0.model Content/Test/Other/man0.zmesh Content/Test/Other/man0_b.tga Content/Test/Other/man0_m.tga Content/Test/Other/man0_n.tga)
source_group(Test\\Other FILES ${Content_Test_Other_GROUP_FILES})

add_custom_target(Content SOURCES ${Content_SRC})


# ========== Executable Game ==========
include_directories(Program/Game)

set(Game_SRC Program/Game/Win32/main.cc)

set(Program_Game_Win32_GROUP_FILES Program/Game/Win32/main.cc)
source_group(Win32 FILES ${Program_Game_Win32_GROUP_FILES})


add_executable(Game ${Game_SRC})
target_link_libraries(Game imgui Engine zlib lua)

set_property(TARGET Game PROPERTY FOLDER Program)


# ========== Executable SLConverter ==========
include_directories(Program/SLConverter)

set(SLConverter_SRC Program/SLConverter/main.cc)



add_executable(SLConverter ${SLConverter_SRC})
target_link_libraries(SLConverter Engine hlslcc)

set_property(TARGET SLConverter PROPERTY FOLDER Program)


# ========== Custom Target BuildTool ==========
set(BuildTool_SRC Program/BuildTool/BuildTool.py Program/BuildTool/BuildTool.pyc Program/BuildTool/CMakeTemplate.py Program/BuildTool/CMakeTemplate.pyc Program/BuildTool/ReflectionTemplates.py Program/BuildTool/Relfect.py)

add_custom_target(BuildTool SOURCES ${BuildTool_SRC})
set_property(TARGET BuildTool PROPERTY FOLDER Program)


# ========== Executable MeshConverter ==========
include_directories(Program/MeshConverter)

set(MeshConverter_SRC Program/MeshConverter/main.cc)



add_executable(MeshConverter ${MeshConverter_SRC})
target_link_libraries(MeshConverter Engine assimp-${MSVC_PREFIX}-mt zlib)

set_property(TARGET MeshConverter PROPERTY FOLDER Program)


# ========== Executable HLSLLint ==========
include_directories(Program/HLSLLint)

set(HLSLLint_SRC Program/HLSLLint/main.cc)



add_executable(HLSLLint ${HLSLLint_SRC})
target_link_libraries(HLSLLint Engine)

set_property(TARGET HLSLLint PROPERTY FOLDER Program)


# ========== Custom Target Config ==========
set(Config_SRC )

add_custom_target(Config SOURCES ${Config_SRC})
