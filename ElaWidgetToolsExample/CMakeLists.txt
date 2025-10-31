cmake_minimum_required(VERSION 3.5)
cmake_policy(SET CMP0087 NEW)
project(ElaWidgetToolsExample VERSION 0.1 LANGUAGES CXX)

FILE(GLOB ORIGIN *.cpp *.h)
FILE(GLOB MODELVIEW ModelView/*.h ModelView/*.cpp)
FILE(GLOB EAXMPLEPAGE ExamplePage/*.h ExamplePage/*.cpp)

source_group(ModelView FILES ${MODELVIEW})
source_group(ExamplePage FILES ${EAXMPLEPAGE})

set(PROJECT_SOURCES
    ${ORIGIN}
    ${MODELVIEW}
    ${EAXMPLEPAGE}
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
    )
else ()
    qt5_add_big_resources(PROJECT_SOURCES
        ElaWidgetToolsExample.qrc
    )
    add_executable(${PROJECT_NAME}
        ${PROJECT_SOURCES}
    )
endif ()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${PROJECT_NAME})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${PROJECT_NAME})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${PROJECT_NAME})

if (WIN32)
    target_include_directories(${PROJECT_NAME} PUBLIC
        ExamplePage
        ModelView
    )
    if (BUILD_ELAPACKETIO)
        target_link_libraries(${PROJECT_NAME} PRIVATE
            Qt${QT_VERSION_MAJOR}::Widgets
            ElaWidgetTools
            ElaPacketIO
        )
    else ()
        target_link_libraries(${PROJECT_NAME} PRIVATE
            Qt${QT_VERSION_MAJOR}::Widgets
            ElaWidgetTools
        )
    endif ()

else ()
    target_include_directories(${PROJECT_NAME} PUBLIC
        ExamplePage
        ModelView
    )
    target_link_libraries(${PROJECT_NAME} PRIVATE
        Qt${QT_VERSION_MAJOR}::Widgets
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
    VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_INSTALL_PREFIX}/${PROJECT_NAME}"
    VS_DEBUGGER_COMMAND "${CMAKE_INSTALL_PREFIX}/${PROJECT_NAME}/${PROJECT_NAME}.exe"
)

if (QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(${PROJECT_NAME})
endif ()


include(GNUInstallDirs)
install(TARGETS ${PROJECT_NAME}
    LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/${PROJECT_NAME}
    RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/${PROJECT_NAME}
)
if (MSVC)
    install(
        FILES $<TARGET_PDB_FILE:${PROJECT_NAME}>
        DESTINATION ${CMAKE_INSTALL_PREFIX}/${PROJECT_NAME} OPTIONAL
    )
endif ()
find_program(WINDEPLOYQT_EXECUTABLE windeployqt HINTS "${QT_SDK_DIR}/bin")
if (WIN32)
    # 先安装核心依赖
    install(FILES $<TARGET_FILE:ElaWidgetTools>
        DESTINATION ${CMAKE_INSTALL_PREFIX}/${PROJECT_NAME}
    )
    
    install(
        FILES ${QT_SDK_DIR}/bin/Qt${QT_VERSION_MAJOR}Core.dll
        FILES ${QT_SDK_DIR}/bin/Qt${QT_VERSION_MAJOR}Gui.dll
        FILES ${QT_SDK_DIR}/bin/Qt${QT_VERSION_MAJOR}Widgets.dll
        DESTINATION ${CMAKE_INSTALL_PREFIX}/${PROJECT_NAME}
    )
    
    install(FILES ${QT_SDK_DIR}/plugins/platforms/qwindows.dll 
        DESTINATION ${CMAKE_INSTALL_PREFIX}/${PROJECT_NAME}/platforms)
    
    # 然后尝试用 windeployqt 补充其他依赖
    find_program(WINDEPLOYQT_EXECUTABLE windeployqt HINTS "${QT_SDK_DIR}/bin")
    if(WINDEPLOYQT_EXECUTABLE)
        install(CODE "
            execute_process(
                COMMAND \"${WINDEPLOYQT_EXECUTABLE}\"
                    \"${CMAKE_INSTALL_PREFIX}/${PROJECT_NAME}/${PROJECT_NAME}.exe\"
                    --verbose 1
                    --no-compiler-runtime
                WORKING_DIRECTORY \"${CMAKE_INSTALL_PREFIX}/${PROJECT_NAME}\"
            )"
        )
    endif()
endif()

