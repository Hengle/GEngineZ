
cmake_minimum_required(VERSION 3.0)

# ========== COMPILE OPTIONS ==========
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/Binary)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/Binary)


# ========== Library Engine ==========
include_directories(Engine)

set(Engine_SRC Engine/Core/Common/common.h Engine/Core/Common/debug.h Engine/Core/Common/Logger.h Engine/Core/Common/Singleton.h Engine/Core/Common/time.h Engine/Core/Scheduler/test.cpp Engine/Core/Scheduler/test.h)

set(Engine_Core_Common_GROUP_FILES Engine/Core/Common/common.h Engine/Core/Common/debug.h Engine/Core/Common/Logger.h Engine/Core/Common/Singleton.h Engine/Core/Common/time.h)
source_group(Core\\Common FILES ${Engine_Core_Common_GROUP_FILES})
set(Engine_Core_Scheduler_GROUP_FILES Engine/Core/Scheduler/test.cpp Engine/Core/Scheduler/test.h)
source_group(Core\\Scheduler FILES ${Engine_Core_Scheduler_GROUP_FILES})

add_library(Engine STATIC ${Engine_SRC})


# ========== Executable Game ==========
include_directories(Program/Game)

set(Game_SRC Program/Game/main.cc)



add_executable(Game ${Game_SRC})
target_link_libraries(Game Engine)