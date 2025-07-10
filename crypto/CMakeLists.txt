cmake_minimum_required(VERSION 3.5)

project(crypto)


#搜索当前目录下的所有源文件
aux_source_directory(./ SRC)

#搜索头文件
file(GLOB HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/*.h)

# 将当前目录下的源文件制作成静态库
add_library(crypto ${SRC} ${HEADERS}
    rsacrypto.h rsacrypto.cpp
    base64.h base64.cpp)

target_link_libraries(crypto PRIVATE Qt${QT_VERSION_MAJOR}::Core
    "D:/AppData/OpenSSL/OpenSSL-Win64/lib/VC/x64/MD/libcrypto.lib"
)
