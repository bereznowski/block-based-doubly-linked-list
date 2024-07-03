cmake_minimum_required(VERSION 3.29)
project(block-based-double-linked-list)

set(SOURCE_FILES
    src/main.cpp
    src/Iterator.cpp
    src/LinkedList.cpp
    src/Node.cpp)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})