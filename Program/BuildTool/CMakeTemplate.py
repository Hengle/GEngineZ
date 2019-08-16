# HEADER 

DEFINE_TEMPLATE = '''\
add_definitions(-D%DEF%)
'''
INCLUDE_TEMPLATE = '''\
include_directories(%INC%)
'''
SRC_GROUP_TEMPLATE = '''\
set(%GRP_SRC_KEY% %GRP_SRC%)
source_group(%GRP_KEY% FILES ${%GRP_SRC_KEY%})
'''


HEADER_TEMPLATE = '''
project(%PROJECT_NAME%)

cmake_minimum_required(VERSION 3.0)

# ========== COMPILE OPTIONS ==========
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_BUILD_TYPE RelWithDebInfo)

set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/Binary)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/Binary)

include_directories(ThirdParty/include)
link_directories(ThirdParty/lib)

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


%DEFINES%
'''

# CUSTOM TARGET
CUSTOM_TEMPLATE = '''
# ========== Custom Target %CUSTOM_NAME% ==========
set(%CUSTOM_NAME%_SRC %CUSTOM_SRCS%)
%CUSTOM_GROUPS%
add_custom_target(%CUSTOM_NAME% SOURCES ${%CUSTOM_NAME%_SRC})
'''

EXECUTABLE_TEMPLATE = '''
# ========== Executable %EXE_NAME% ==========
%INCLUDE_EXE_DIR%

set(%EXE_NAME%_SRC %EXE_SRCS%)

%EXE_GROUPS%

add_executable(%EXE_NAME% ${%EXE_NAME%_SRC})
target_link_libraries(%EXE_NAME% %DEP_LIBS%)
'''

LIBRARY_TEMPLATE = '''
# ========== Library %LIB_NAME% ==========
%INCLUDE_LIB_DIR%

set(%LIB_NAME%_SRC %LIB_SRCS%)

%LIB_GROUPS%

add_library(%LIB_NAME% STATIC ${%LIB_NAME%_SRC})
'''