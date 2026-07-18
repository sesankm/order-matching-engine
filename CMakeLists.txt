cmake_minimum_required(VERSION 4.0.0)
project(order-matching-engine CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED  ON)

include_directories(include)

set(SRC
        src/client.cpp
        src/server.cpp
        src/order.cpp
        src/lob.cpp
        src/peer.cpp
        src/peer.cpp
        src/matching_engine.cpp
        src/message_parser.cpp
        src/ring_buffer.cpp
)


add_executable(server src/run_server.cpp ${SRC})
add_executable(client src/run_client.cpp ${SRC})

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS_DEBUG "-g -fsanitize=address,undefined -fno-omit-frame-pointer")
set(CMAKE_EXE_LINKER_FLAGS_DEBUG "-fsanitize=address,undefined")

add_executable(lobtest   test/lob_test.cpp   ${SRC})
add_executable(matchtest test/match_test.cpp ${SRC})

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/heads/main.zip
)

FetchContent_MakeAvailable(googletest)
add_executable(mp_test test/test_message_parser.cpp ${SRC})
target_link_libraries(mp_test PRIVATE gtest_main)

add_executable(rb_test test/ring_buffer_test.cpp ${SRC})
target_link_libraries(rb_test PRIVATE gtest_main)
