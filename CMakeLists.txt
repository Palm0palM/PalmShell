cmake_minimum_required(VERSION 3.28)
project(PalmShell)

set(CMAKE_CXX_STANDARD 20)

add_executable(PalmShell psh.cpp fun_wrappers.cpp)

target_link_libraries(PalmShell readline)