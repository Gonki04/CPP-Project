cmake_minimum_required(VERSION 3.29)
project(CPP_Project)

# Copy resources
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/resources DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

# Find packages
find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)

# Add subdirectories
add_subdirectory(glad/)
add_subdirectory(extern/glew)

# Set C++ standard
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)

# Recursive file search with CONFIGURE_DEPENDS (CMake 3.12+)
set(SOURCE_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/src")

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
    "${SOURCE_ROOT}/*.cpp"
    "${SOURCE_ROOT}/*.c"
)

file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS
    "${SOURCE_ROOT}/*.h"
    "${SOURCE_ROOT}/*.hpp"
    "${SOURCE_ROOT}/*.hxx"
)

# Create executable
add_executable(CPP_Project ${SOURCES} ${HEADERS})

# Link libraries
target_link_libraries(CPP_Project PRIVATE
    glfw
    glm::glm
    libglew_static
    glad
)

# Include directories - add root and all subdirectories
target_include_directories(CPP_Project PRIVATE
    ${SOURCE_ROOT}
    extern/glew/include
    extern/stb_image
)

# Windows-specific resource copying
if(WIN32)
    add_custom_command(
        TARGET CPP_Project POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_CURRENT_SOURCE_DIR}/resources
            $<TARGET_FILE_DIR:CPP_Project>/resources
    )
endif()