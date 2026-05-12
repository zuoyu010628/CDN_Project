add_library(logger STATIC 
    src/async_logger.cpp
)

target_include_directories(logger PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(logger PUBLIC 
    Threads::Threads
)