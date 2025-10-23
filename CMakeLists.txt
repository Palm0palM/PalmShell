cmake_minimum_required(VERSION 3.28)
project(PalmShell)
set(CMAKE_CXX_STANDARD 20)

include_directories(${PROJECT_SOURCE_DIR}/include)

add_executable(PalmShell src/psh.cpp src/fun_wrappers.cpp)
target_link_libraries(PalmShell readline)