cmake_minimum_required(VERSION 3.5)

project(ddz_client VERSION 0.1 LANGUAGES CXX)

#自动处理ui文件，最后生成一个ui.xxx.h文件
set(CMAKE_AUTOUIC ON)
#自动处理元对象相关的宏 Q_OBJECT
set(CMAKE_AUTOMOC ON)
#自动处理qrc文件，会生成相应的C++代码，最后嵌入到可执行程序中
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Multimedia)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Multimedia)

set(PROJECT_SOURCES
        main.cpp
)


#添加头文件目录,须写到添加子目录的上面
include_directories("D:/AppData/OpenSSL/OpenSSL-Win64/include")
include_directories("D:/AppData/Protoc/install/include")
include_directories(
    ${PROJECT_SOURCE_DIR}/card
    ${PROJECT_SOURCE_DIR}/windows
    ${PROJECT_SOURCE_DIR}/control
    ${PROJECT_SOURCE_DIR}/player
    ${PROJECT_SOURCE_DIR}/strategy
    ${PROJECT_SOURCE_DIR}/thread
    ${PROJECT_SOURCE_DIR}/tcp
    ${PROJECT_SOURCE_DIR}/serialize
    ${PROJECT_SOURCE_DIR}/crypto
)

link_directories("D:/AppData/Protoc/protobuf_QT_minggw/bin")

# 添加子目录
add_subdirectory(card)
add_subdirectory(windows)
add_subdirectory(control)
add_subdirectory(player)
add_subdirectory(strategy)
add_subdirectory(thread)
add_subdirectory(tcp)
add_subdirectory(serialize)
add_subdirectory(crypto)

#添加资源文件
qt5_add_resources(RES res.qrc)

#添加图标资源文件
if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    enable_language("RC")
    set(ICON_RES ${CMAKE_CURRENT_SOURCE_DIR}/ddz.rc)
endif()

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(ddz_client
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET ddz_client APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(ddz_client SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(ddz_client
            ${PROJECT_SOURCES}
            ${RES}
            ${ICON_RES}
        )
    endif()
endif()

target_link_libraries(ddz_client PRIVATE Qt${QT_VERSION_MAJOR}::Widgets
    card
    windows
    control
    player
    strategy
    thread
    crypto
    serialize
    tcp
    Qt${QT_VERSION_MAJOR}::Multimedia
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.ddz_client)
endif()
set_target_properties(ddz_client PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS ddz_client
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(ddz_client)
endif()
