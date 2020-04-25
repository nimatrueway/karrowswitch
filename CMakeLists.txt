cmake_minimum_required(VERSION 3.0)
project(pause-change-fixer)

set(CMAKE_CXX_STANDARD 17)

add_executable(pause-change-fixer pause-change-fixer.cpp)
target_link_libraries(pause-change-fixer -lXtst -lX11 -pthread)
install(TARGETS pause-change-fixer DESTINATION /usr/bin/)