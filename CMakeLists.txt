cmake_minimum_required(VERSION 3.20)

project(ThreadPrint LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Threads REQUIRED)
find_package(GTest REQUIRED)

# TCP Server
add_executable(threadprint_server
    src/main.cpp
    src/JobQueue.cpp
    src/ThreadPool.cpp
)

target_include_directories(threadprint_server
    PRIVATE include
)

target_link_libraries(threadprint_server
    PRIVATE Threads::Threads
)

# TCP Client
add_executable(threadprint_client
    client/client.cpp
)

# Unit Tests
enable_testing()

add_executable(test_job_queue
    tests/test_job_queue.cpp
    src/JobQueue.cpp
)

target_include_directories(test_job_queue
    PRIVATE include
)

target_link_libraries(test_job_queue
    PRIVATE
    GTest::gtest_main
    Threads::Threads
)

include(GoogleTest)

gtest_discover_tests(test_job_queue)