cmake_minimum_required(VERSION 3.12)

project(InvertedIndex)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

file(GLOB SOURCES "src/*.cc")
file(GLOB CLIENT_SOURCES "src/client/*.cc")

add_executable(${PROJECT_NAME} ${SOURCES})
add_executable("Client" ${CLIENT_SOURCES})

set_target_properties(${PROJECT_NAME} "Client" PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra)
