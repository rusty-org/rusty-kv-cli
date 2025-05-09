cmake_minimum_required(VERSION 3.14)
project(rusty_kv_cli)

# Use C++17 or newer
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include directories
include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/client)
include_directories(${PROJECT_SOURCE_DIR}/utils)

# Collect all .cpp files
file(GLOB_RECURSE SOURCES
    ${PROJECT_SOURCE_DIR}/*.cpp
)

# Output directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/tmp)

# ICU dependency (as per your Makefile)
find_package(ICU REQUIRED COMPONENTS uc io)

# Executable
add_executable(cli ${SOURCES})
target_link_libraries(cli ICU::uc ICU::io)
