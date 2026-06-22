cmake_minimum_required(VERSION 3.20)
set(CMAKE_POLICY_DEFAULT_CMP0000 NEW)
set(CMAKE_POLICY_DEFAULT_CMP0048 NEW)
project(Frostbytes VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)
set(FETCHCONTENT_QUIET FALSE)

# GLFW
FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG        3.3.8
)
set(GLFW_BUILD_DOCS     OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS    OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL        OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(glfw)

# GLAD 2
FetchContent_Declare(
    glad2
    GIT_REPOSITORY https://github.com/Dav1dde/glad.git
    GIT_TAG        v2.0.5
    SOURCE_SUBDIR  cmake
)
FetchContent_MakeAvailable(glad2)
glad_add_library(glad_gl STATIC LANGUAGE C API gl:core=3.3)

# GLM
FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG        0.9.9.8
)
FetchContent_MakeAvailable(glm)

file(GLOB_RECURSE FB_SOURCES "src/*.cpp")
add_executable(Frostbytes ${FB_SOURCES})

target_link_libraries(Frostbytes PRIVATE glfw glad_gl glm::glm)
target_include_directories(Frostbytes PRIVATE src vendor)

# Copy assets folder next to the executable after every build
add_custom_command(TARGET Frostbytes POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/assets"
        "$<TARGET_FILE_DIR:Frostbytes>/assets"
)

if(MSVC)
    target_compile_options(Frostbytes PRIVATE /W4)
else()
    target_compile_options(Frostbytes PRIVATE -Wall -Wextra -Wpedantic)
endif()
