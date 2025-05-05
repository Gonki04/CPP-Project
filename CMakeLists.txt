cmake_minimum_required(VERSION 3.29)
project(CPP_Project)

# Check if we are on Windows


    # Use file copy for non-Windows platforms (Linux, macOS, etc.)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/resources DESTINATION ${CMAKE_CURRENT_BINARY_DIR})


# Find the necessary packages
find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)

# Add glad subdirectory
add_subdirectory(glad/)
add_subdirectory(extern/glew)  # GLEW's CMake build system
# Set C++ standard
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)


file(GLOB SOURCES "src/*.cpp")

# Add the executable target
add_executable(CPP_Project ${SOURCES})

# Link libraries to the executable
target_link_libraries(CPP_Project PRIVATE glfw)
target_link_libraries(CPP_Project PRIVATE glm::glm)
target_link_libraries(CPP_Project PRIVATE libglew_static)

target_link_libraries(CPP_Project PRIVATE glad)
# --- Include GLEW headers ---
target_include_directories(CPP_Project PRIVATE
    extern/glew/include  # GLEW headers
    src/header
)



if(WIN32)
    # Use custom command to copy resources folder on Windows
    add_custom_command(
        TARGET CPP_Project POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_CURRENT_SOURCE_DIR}/resources
            $<TARGET_FILE_DIR:CPP_Project>/resources
    )
endif()
