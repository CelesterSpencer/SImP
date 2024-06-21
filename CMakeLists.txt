cmake_minimum_required(VERSION 3.14)
project(MyProject)

# set paths
set(PROJECT_SRC ${CMAKE_SOURCE_DIR}/src)
set(EXTERNAL_DIR ${CMAKE_CURRENT_SOURCE_DIR}/deps)

# add defines
add_definitions(-DSHADERS_PATH="${SHADERS_PATH}")
add_definitions(-DRESOURCES_PATH="${RESOURCES_PATH}")
add_definitions(-DDEPENDENCIES_PATH="${EXTERNAL_PATH}")

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
#SET(BUILD_SHARED_LIBRARIES OFF)
#SET(CMAKE_EXE_LINKER_FLAGS "-static")

# Define the project source directory
set(SRC_DIR "${CMAKE_SOURCE_DIR}/src")

# Enable FetchContent module
include(FetchContent)

# Fetch glfw
FetchContent_Declare(
        glfw
        GIT_REPOSITORY https://github.com/glfw/glfw.git
        GIT_TAG        3.3.4
)
FetchContent_MakeAvailable(glfw)

# Fetch glad
FetchContent_Declare(
        glad
        GIT_REPOSITORY https://github.com/Dav1dde/glad.git
        GIT_TAG        v0.1.34
)
FetchContent_MakeAvailable(glad)

# Fetch glm
FetchContent_Declare(
        glm
        GIT_REPOSITORY https://github.com/g-truc/glm.git
        GIT_TAG        0.9.9.8
)
FetchContent_MakeAvailable(glm)

# Fetch stb_image
FetchContent_Declare(
        stb_image
        GIT_REPOSITORY https://github.com/nothings/stb.git
        GIT_TAG        master
)
FetchContent_GetProperties(stb_image)
if(NOT stb_image_POPULATED)
    FetchContent_Populate(stb_image)
    add_library(stb_image INTERFACE)
    target_include_directories(stb_image INTERFACE ${stb_image_SOURCE_DIR})
endif()

# Fetch imgui
FetchContent_Declare(
        imgui
        GIT_REPOSITORY https://github.com/ocornut/imgui.git
        GIT_TAG        v1.89.3
)
FetchContent_MakeAvailable(imgui)
file(GLOB imgui_SRC ${imgui_SOURCE_DIR}/*.cpp ${imgui_SOURCE_DIR}/*.h)
add_library(
        imgui
        ${imgui_SRC}
        ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
        ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.h
        ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
        ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.h
)

target_link_libraries(imgui PUBLIC glad)
if (NOT EMSCRIPTEN)
    target_link_libraries(imgui PUBLIC glfw)
endif ()
target_include_directories(
        imgui
        PUBLIC
        ${imgui_SOURCE_DIR}
        ${imgui_SOURCE_DIR}/backends
)

# Add the executable
file(GLOB_RECURSE SRC_FILES "${SRC_DIR}/*.cpp" "${SRC_DIR}/*.h")
add_executable(MyProject ${SRC_FILES})

# Include directories
target_include_directories(MyProject PRIVATE
        ${PROJECT_SRC}
        ${glfw_SOURCE_DIR}/include
        ${glad_SOURCE_DIR}/include
        ${glm_SOURCE_DIR}
        ${stb_image_SOURCE_DIR}
        ${imgui_SOURCE_DIR}
)

# Link libraries
target_link_libraries(MyProject PRIVATE
        glfw
        glad
        glm
        stb_image
        imgui
)

# Link system libraries
if(WIN32)
    target_link_libraries(MyProject PRIVATE opengl32)
elseif(APPLE)
    find_library(COCOA_LIBRARY Cocoa REQUIRED)
    find_library(OpenGL_LIBRARY OpenGL REQUIRED)
    find_library(IOKit_LIBRARY IOKit REQUIRED)
    find_library(CoreVideo_LIBRARY CoreVideo REQUIRED)
    target_link_libraries(MyProject PRIVATE ${COCOA_LIBRARY} ${OpenGL_LIBRARY} ${IOKit_LIBRARY} ${CoreVideo_LIBRARY})
else()
    find_package(OpenGL REQUIRED)
    target_link_libraries(MyProject PRIVATE ${OPENGL_LIBRARIES})
endif()

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
add_dependencies(MyProject copy_shaders copy_images)