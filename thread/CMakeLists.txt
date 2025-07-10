cmake_minimum_required(VERSION 3.5)

project(thread)

#搜索当前目录下的所有源文件
aux_source_directory(./ SRC)

#搜索头文件
file(GLOB HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/*.h)

# 将当前目录下的源文件制作成静态库
add_library(thread ${SRC} ${HEADERS}
    taskqueue.h taskqueue.cpp)

target_link_libraries(thread PRIVATE Qt${QT_VERSION_MAJOR}::Core)
