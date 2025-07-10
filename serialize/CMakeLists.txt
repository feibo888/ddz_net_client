cmake_minimum_required(VERSION 3.5)

project(serialize)

#搜索当前目录下的所有源文件
aux_source_directory(./ SRC)

#搜索头文件
file(GLOB HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/*.h)
file(GLOB PROTO ${CMAKE_CURRENT_SOURCE_DIR}/*.proto)


# 将当前目录下的源文件制作成静态库
add_library(serialize ${SRC} ${HEADERS} ${PROTO}
    codec.h codec.cpp)

target_link_libraries(serialize PRIVATE Qt${QT_VERSION_MAJOR}::Core protobufd)
