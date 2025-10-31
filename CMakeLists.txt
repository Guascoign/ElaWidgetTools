cmake_minimum_required(VERSION 3.5)

project(ElaFramework VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

#在这里配置QT路径 例如 D:/Qt/6.6.2/msvc2019_64 D:/Qt/5.15.2/msvc2019_64 /home/liniyous/Qt/5.15.2/gcc_64
SET(QT_SDK_DIR D:/Qt/5.15.2/msvc2019_64 CACHE PATH "QT SDK DIR" FORCE)
message("Configure QT_SDK_DIR option in CMAKE to specify QT path; Current path: ${QT_SDK_DIR}")
SET(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/Install CACHE PATH "Installation path" FORCE)

option(BUILD_ELAPACKETIO "Build ElaPacketIO" OFF)
list(APPEND CMAKE_PREFIX_PATH ${QT_SDK_DIR})

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

if (${QT_VERSION} VERSION_GREATER 6.7.0 OR ${QT_VERSION} VERSION_LESS 5.12.0)
message("The QT version you are currently using is no longer under maintenance! There may be issues such as compilation failures or crashes during use. The recommended QT versions are QT5.15.2, QT6.6.2, and QT6.6.3! Please be advised")
endif ()

if (${QT_VERSION} VERSION_GREATER_EQUAL 6.5.3 AND ${QT_VERSION} VERSION_LESS_EQUAL 6.6.1)
message("The QT version you are currently using is not recommended! It may cause exceptions or crashes during operation. The recommended QT versions are QT5.15.2, QT6.6.2, and QT6.6.3! Please be advised")
endif ()

if (NOT WIN32)
    add_link_options(-Wl,--disable-new-dtags)
    set(CMAKE_SKIP_INSTALL_RPATH FALSE)
    set(CMAKE_INSTALL_RPATH "${QT_SDK_DIR}/lib:${CMAKE_INSTALL_PREFIX}/ElaWidgetTools/lib")
endif ()

add_subdirectory(ElaWidgetTools)
if (WIN32 AND BUILD_ELAPACKETIO)
    add_definitions(-DBUILD_WITH_ELAPACKETIO)
    add_subdirectory(ElaPacketIO)
endif ()
add_subdirectory(ElaWidgetToolsExample)


# 读取version.h中的版本信息
file(READ "ElaWidgetToolsExample/version.h" VERSION_FILE_CONTENT)
string(REGEX MATCH "VER_PRODUCTVERSION[ \t]+([0-9]+),([0-9]+),([0-9]+),([0-9]+)" _ ${VERSION_FILE_CONTENT})
set(VERSION_MAJOR ${CMAKE_MATCH_1})
set(VERSION_MINOR ${CMAKE_MATCH_2})
set(VERSION_PATCH ${CMAKE_MATCH_3})
set(VERSION_BUILD ${CMAKE_MATCH_4})

# 构建版本字符串
set(VERSION_STRING "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")

# 设置exe输出名称
set_target_properties(ElaWidgetToolsExample PROPERTIES
    OUTPUT_NAME "ZMTools_V${VERSION_STRING}"
)


if (${QT_VERSION} VERSION_LESS 6.1.0)
    set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.ElaWidgetTools)
endif ()
