cmake_minimum_required(VERSION 3.0)
project(karrowswitch)

set(CMAKE_CXX_STANDARD 17)

add_executable(karrowswitch karrowswitch.cpp)
target_link_libraries(karrowswitch -lXtst -lX11 -pthread)
install(TARGETS karrowswitch DESTINATION /usr/bin/)