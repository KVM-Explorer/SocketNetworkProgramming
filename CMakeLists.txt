project("SocketNetworkProgramming")
cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)
set(TARGET_NAME "UnitTest")
file(GLOB_RECURSE TARGET_SOURCE "DataLinkLayer/*.cpp" "NetworkLayer/*.cpp" "TransportLayer/*.cpp")
file(GLOB_RECURSE TARGET_HEADER "DataLinkLayer/*.h" "NetworkLayer/*.h" "TransportLayer/*.h")
file(GLOB_RECURSE TARGET_TEST "UnitTest/*")

foreach(TARGET ${TARGET_TEST})
    get_filename_component(TARGET_NAME ${TARGET} NAME_WE)
    message(STATUS "Building ${TARGET_NAME}")
    add_executable(${TARGET_NAME} ${TARGET} ${TARGET_HEADER} ${TARGET_SOURCE})

    # In Linux, 'Threads' need to explicitly linked.
    if(CMAKE_SYSTEM_NAME MATCHES "Linux")
        find_package(Threads REQUIRED)
        target_link_libraries(${TARGET_NAME} PUBLIC ${CMAKE_THREAD_LIBS_INIT})
    endif()
endforeach(TARGET)


