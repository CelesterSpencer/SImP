project(srccmake)
cmake_minimum_required(VERSION 2.8)

# set flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake CACHE PATH "Project specific path. Set manually if it was not found.")
SET(BUILD_SHARED_LIBRARIES OFF)
SET(CMAKE_EXE_LINKER_FLAGS "-static")

# Compiler flags release mode
IF(CMAKE_BUILD_TYPE MATCHES RELEASE)
    message("release mode")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mwindows")
ENDIF(CMAKE_BUILD_TYPE MATCHES RELEASE)

## Compiler flags gcc
if(CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")        ## Optimize
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s")  ## Strip binary
endif()

# set paths
set(PROJECT_SRC ${CMAKE_SOURCE_DIR}/src)
set(EXTERNAL_DIR ${CMAKE_CURRENT_SOURCE_DIR}/deps)
#set(SHADERS_PATH ${CMAKE_SOURCE_DIR}/shader)
#set(RESOURCES_PATH ${CMAKE_SOURCE_DIR}/resources)

# add defines
add_definitions(-DSHADERS_PATH="${SHADERS_PATH}")
add_definitions(-DRESOURCES_PATH="${RESOURCES_PATH}")
add_definitions(-DDEPENDENCIES_PATH="${EXTERNAL_PATH}")

# find dependencies
find_package(GLFW3 REQUIRED)
find_package(GLM REQUIRED)

# set includes
include_directories(
        ${PROJECT_SRC}
        ${GLFW3_INCLUDE_PATH}
        ${GLM_INCLUDE_PATH}
        ${EXTERNAL_DIR}/gl3w/include
        ${EXTERNAL_DIR}/imgui
        ${EXTERNAL_DIR}/stb
        ${EXTERNAL_DIR}/mingw_thread
)

# copy dlls
set(DLLS
        ${GLFW3_DLL}
)
file(COPY ${DLLS} DESTINATION /)

# copy other files
file(COPY shader DESTINATION /)

# add source files
file(GLOB_RECURSE SOURCE_FILES
        "${PROJECT_SRC}/*.cpp"
)

# add gl3w
file(GLOB_RECURSE GL3W
        "${EXTERNAL_DIR}/gl3w/src/*.c"
)

# add stb_image
file(GLOB_RECURSE STB
        "${EXTERNAL_DIR}/stb/*.h"
        "${EXTERNAL_DIR}/stb/*.c"
)

# add imgui
file(GLOB_RECURSE IMGUI
        "${EXTERNAL_DIR}/imgui/*.h"
        "${EXTERNAL_DIR}/imgui/*.cpp"
)

# add dependencies and source files to the project
add_executable(SImP
        ${SOURCE_FILES}
        ${GL3W}
        ${STB}
        ${IMGUI}
)

# link all libraries
target_link_libraries(SImP
        opengl32.lib
        ${GLFW3_LIBRARY}
		# libs required on windows for window management
		# and graphics programming
        ole32.lib
        oleaut32.lib
        version.lib
        imm32.lib
        winmm.lib
        Shlwapi.lib
        #-mwindows
)

# copy shader files every time the project is built
add_custom_target(copy_shaders ALL
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/shader" "${CMAKE_BINARY_DIR}/shader"
        COMMENT "Copied shaders from ${CMAKE_SOURCE_DIR}/shader to ${CMAKE_BINARY_DIR}/shader" VERBATIM
)

# copy images
add_custom_target(copy_images ALL
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/resources" "${CMAKE_BINARY_DIR}/resources"
        COMMENT "Copied resources from ${CMAKE_SOURCE_DIR}/resources to ${CMAKE_BINARY_DIR}/resources" VERBATIM
)