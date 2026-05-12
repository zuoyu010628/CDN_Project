cmake_minimum_required(VERSION 3.14)
project(AsyncServer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Threads REQUIRED)

add_subdirectory(logger)
add_subdirectory(thread_pool)

add_executable(main main.cpp)

target_include_directories(main PRIVATE  
    ${CMAKE_CURRENT_SOURCE_DIR}/test
)

target_link_libraries(main PRIVATE 
    logger 
    thread_pool
)
