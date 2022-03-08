project("SocketNetworkProgramming")
cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)
set(TARGET_NAME "UnitTest")
file(GLOB_RECURSE TARGET_SOURCE "DataLinkLayer/*.cpp" "NetworkLayer/*.cpp")
file(GLOB_RECURSE TARGET_HEADER "DataLinkLayer/*.h" "NetworkLayer/*.h")

add_executable(${TARGET_NAME}  ${TARGET_HEADER} ${TARGET_SOURCE}  test.cpp NetworkLayer/ArpRequest.cpp NetworkLayer/ArpRequest.h NetworkLayer/ArpReply.cpp NetworkLayer/ArpReply.h)

# In Linux, 'Threads' need to explicitly linked.
if(CMAKE_SYSTEM_NAME MATCHES "Linux")
    find_package(Threads REQUIRED)
    target_link_libraries(${TARGET_NAME} PUBLIC ${CMAKE_THREAD_LIBS_INIT})
endif()
