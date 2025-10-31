cmake_minimum_required(VERSION 3.5)

project(ElaWidgetToolsExample VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS SerialPort )#串口
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS PrintSupport)#Qcustomplot

FILE(GLOB ORIGIN *.cpp *.h)
FILE(GLOB MODELVIEW ModelView/*.h ModelView/*.cpp)
FILE(GLOB EAXMPLEPAGE ExamplePage/*.h ExamplePage/*.cpp)
FILE(GLOB PAGE Page/*.h Page/*.cpp)
FILE(GLOB APPSRC Appsrc/*.h Appsrc/*.cpp)
FILE(GLOB FSM_CONTROL Appsrc/FSM_Control/*.h Appsrc/FSM_Control/*.cpp)
FILE(GLOB Proctol_T Appsrc/Proctol_T/*.h Appsrc/Proctol_T/*.cpp)
FILE(GLOB_RECURSE THIRDPARTY Third_party/*.h Third_party/*.cpp)#递归包含子目录中的文件

source_group(ModelView FILES ${MODELVIEW})
source_group(ExamplePage FILES ${EAXMPLEPAGE})
source_group(Page FILES ${PAGE})
source_group(Appsrc FILES ${APPSRC})
source_group(FSM_Control FILES ${FSM_CONTROL})
source_group(Proctol_T FILES ${Proctol_T})
source_group(ThirdParty FILES ${THIRDPARTY})

# Windows 资源文件
if (WIN32)
    set(RESOURCE_FILES
        app.rc
        version.h
    )
endif()

set(PROJECT_SOURCES
    ${ORIGIN}
    ${MODELVIEW}
    ${EAXMPLEPAGE}
    ${PAGE}
    ${APPSRC}
    ${FSM_CONTROL}
    ${Proctol_T}
    ${RESOURCE_FILES}
    ${THIRDPARTY}
)

if (${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(${PROJECT_NAME}
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
    #遍历所有资源文件
    file(GLOB_RECURSE RES_PATHS *.png *.jpg *.svg *.ico *.ttf *.webp *.js)
    foreach (filepath ${RES_PATHS})
        string(REPLACE "${CMAKE_CURRENT_SOURCE_DIR}/" "" filename ${filepath})
        list(APPEND resource_files ${filename})
    endforeach (filepath)

    qt_add_resources(${PROJECT_NAME} "ElaWidgetToolsExample"
        RESOURCES PREFIX "/"
        FILES
        ${resource_files}
        #ElaWidgetToolsExample.qrc
    )
else ()
    qt5_add_big_resources(PROJECT_SOURCES
        ElaWidgetToolsExample.qrc
    )
    add_executable(${PROJECT_NAME}
        ${PROJECT_SOURCES}
    )
endif ()

add_custom_command(TARGET ElaWidgetToolsExample POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "$<TARGET_FILE:ElaWidgetTools>"
        "$<TARGET_FILE_DIR:ElaWidgetToolsExample>"
)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${PROJECT_NAME})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${PROJECT_NAME})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${PROJECT_NAME})

if (WIN32)
    target_include_directories(${PROJECT_NAME} PUBLIC
        ExamplePage
        Page
        Appsrc
        Appsrc/FSM_Control
        Appsrc/Proctol_T
        ModelView
        Third_party
    )
    # if (BUILD_ELAPACKETIO)
    #     target_link_libraries(${PROJECT_NAME} PRIVATE
    #         Qt${QT_VERSION_MAJOR}::Widgets
    #         ElaWidgetTools
    #         ElaPacketIO
    #     )
    # else ()
        target_link_libraries(${PROJECT_NAME} PRIVATE
            Qt${QT_VERSION_MAJOR}::Widgets
            Qt${QT_VERSION_MAJOR}::Core
            Qt${QT_VERSION_MAJOR}::SerialPort
            Qt${QT_VERSION_MAJOR}::PrintSupport
            ElaWidgetTools
         )
    #endif ()

else ()
    target_include_directories(${PROJECT_NAME} PUBLIC
        ExamplePage
        Page
        Appsrc
        Appsrc/FSM_Control
        Appsrc/Proctol_T
        ModelView
        Third_party
    )
    target_link_libraries(${PROJECT_NAME} PRIVATE
        Qt${QT_VERSION_MAJOR}::Widgets
        Qt${QT_VERSION_MAJOR}::Core
        Qt${QT_VERSION_MAJOR}::SerialPort
        Qt${QT_VERSION_MAJOR}::printsupport
        ElaWidgetTools
    )
endif ()

if (${QT_VERSION} VERSION_LESS 6.1.0)
    set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.${PROJECT_NAME})
endif ()
set_target_properties(${PROJECT_NAME} PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

if (QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(${PROJECT_NAME})
endif ()

if (WIN32)
    include(GNUInstallDirs)
    install(TARGETS ${PROJECT_NAME}
        LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/${PROJECT_NAME}
        RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/${PROJECT_NAME}
    )
    if (MSVC)
        install(
            FILES $<TARGET_PDB_FILE:${PROJECT_NAME}>
            DESTINATION ${CMAKE_INSTALL_PREFIX}/${PROJECT_NAME} OPTIONAL)
    endif ()
    
    # 添加 ElaWidgetTools.dll 的安装
    install(FILES $<TARGET_FILE:ElaWidgetTools>
        DESTINATION ${CMAKE_INSTALL_PREFIX}/${PROJECT_NAME}
    )
    
    # QT依赖安装 只做Release安装 Debug自行打包
    install(FILES ${QT_SDK_DIR}/plugins/platforms/qwindows.dll DESTINATION ${CMAKE_INSTALL_PREFIX}/${PROJECT_NAME}/platforms)
    install(FILES ${QT_SDK_DIR}/plugins/generic/qtuiotouchplugin.dll DESTINATION ${CMAKE_INSTALL_PREFIX}/${PROJECT_NAME}/generic)
    install(
        FILES ${QT_SDK_DIR}/plugins/imageformats/qgif.dll
        FILES ${QT_SDK_DIR}/plugins/imageformats/qicns.dll
        FILES ${QT_SDK_DIR}/plugins/imageformats/qico.dll
        FILES ${QT_SDK_DIR}/plugins/imageformats/qjpeg.dll
        FILES ${QT_SDK_DIR}/plugins/imageformats/qtga.dll
        FILES ${QT_SDK_DIR}/plugins/imageformats/qtiff.dll
        FILES ${QT_SDK_DIR}/plugins/imageformats/qwbmp.dll
        FILES ${QT_SDK_DIR}/plugins/imageformats/qwebp.dll
        DESTINATION ${CMAKE_INSTALL_PREFIX}/${PROJECT_NAME}/imageformats
    )
    install(FILES ${QT_SDK_DIR}/plugins/styles/qwindowsvistastyle.dll DESTINATION ${CMAKE_INSTALL_PREFIX}/${PROJECT_NAME}/styles)
    FILE(GLOB TRANSLATIONS ${QT_SDK_DIR}/translations_install/*.qm)
    install(FILES ${TRANSLATIONS} DESTINATION ${CMAKE_INSTALL_PREFIX}/${PROJECT_NAME}/translations)
    install(
        FILES ${QT_SDK_DIR}/bin/Qt${QT_VERSION_MAJOR}Widgets.dll
        FILES ${QT_SDK_DIR}/bin/Qt${QT_VERSION_MAJOR}Core.dll
        FILES ${QT_SDK_DIR}/bin/Qt${QT_VERSION_MAJOR}Gui.dll
        FILES ${QT_SDK_DIR}/bin/Qt${QT_VERSION_MAJOR}Network.dll
        FILES ${QT_SDK_DIR}/bin/Qt${QT_VERSION_MAJOR}Xml.dll
        DESTINATION ${CMAKE_INSTALL_PREFIX}/${PROJECT_NAME}
    )
endif ()

