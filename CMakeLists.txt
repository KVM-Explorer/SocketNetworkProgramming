project("SocketNetworkProgramming")
cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)
set(TARGET_NAME "UnitTest")
file(GLOB_RECURSE TARGET_SOURCE "DataLinkLayer/*.cpp")
file(GLOB_RECURSE TARGET_HEADER "DataLinkLayer/*.h")

add_executable(${TARGET_NAME}  test.cpp ${TARGET_HEADER} ${TARGET_SOURCE} )

