cmake_minimum_required(VERSION 3.20)

project(WorldConv VERSION 3.0 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets LinguistTools)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets LinguistTools)

set(TS_FILES resources/WorldConv_en_US.ts)

file(GLOB UI_FILES resources/*.ui)

file(GLOB_RECURSE SOURCE_FILES src/*)

set(PROJECT_SOURCES
        ${SOURCE_FILES}
        ${UI_FILES}
        ${TS_FILES}
)

qt_add_executable(WorldConv
    MANUAL_FINALIZATION
    ${PROJECT_SOURCES}
)

qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})

target_link_libraries(WorldConv PRIVATE
    Qt6::Widgets
    Qt6::Core
    Qt6::Gui
)

target_compile_definitions(
    WorldConv
    PUBLIC
        PROJECT_VERSION=${PROJECT_VERSION})

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(WorldConv)
endif()

set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/bin)
qt_generate_deploy_app_script(
    TARGET WorldConv
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
