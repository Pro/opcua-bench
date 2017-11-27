cmake_minimum_required(VERSION 3.0)
project(opcua-bench)

add_subdirectory(deps/open62541)

include_directories(${PROJECT_BINARY_DIR}/deps/open62541)

add_executable(throughput throughput.c)
target_link_libraries(throughput open62541 ${open62541_LIBRARIES})