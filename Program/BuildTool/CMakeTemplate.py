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

set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/Binary)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/Binary)

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