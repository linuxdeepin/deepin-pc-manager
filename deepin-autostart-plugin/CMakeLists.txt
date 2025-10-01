cmake_minimum_required(VERSION 3.7)

set(PLUGIN_NAME "deepin-autostart")

project(${PLUGIN_NAME})

set(CMAKE_AUTOMOC ON)

file(GLOB_RECURSE SRCS
    "*.h"
    "*.cpp")

find_package(Qt5 COMPONENTS Core Widgets DBus Svg LinguistTools REQUIRED)
find_package(PkgConfig REQUIRED)
find_package(DtkWidget REQUIRED)

pkg_check_modules(DFrameworkDBus REQUIRED dframeworkdbus)
pkg_check_modules(QGSettings REQUIRED gsettings-qt)

file(GLOB TS_FILES "translations/*.ts")
qt5_add_translation(QM_FILES ${TS_FILES})
add_custom_target(translationsautostart ALL DEPENDS ${QM_FILES})
install(FILES ${QM_FILES} DESTINATION share/${PLUGIN_NAME}/translations)

add_library(${PLUGIN_NAME} SHARED ${SRCS} icons/autostart_icons.qrc)
set_target_properties(${PLUGIN_NAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY ./)
target_include_directories(${PLUGIN_NAME} PUBLIC
    ${Qt5Widgets_INCLUDE_DIRS}
    ${Qt5Svg_INCLUDE_DIRS}
    ${DtkWidget_INCLUDE_DIRS}
    ${QGSettings_INCLUDE_DIRS}
    ${Qt5DBus_INCLUDE_DIRS}
    ../deepin-pc-manager/include/
    )
target_link_libraries(${PLUGIN_NAME} PRIVATE
    ${Qt5Widgets_LIBRARIES}
    ${Qt5Svg_LIBRARIES}
    ${DtkWidget_LIBRARIES}
    ${DFrameworkDBus_LIBRARIES}
    ${QGSettings_LIBRARIES}
    ${Qt5DBus_LIBRARIES}
    )

install(TARGETS ${PLUGIN_NAME} LIBRARY DESTINATION /usr/lib/deepin-pc-manager/modules)
install(FILES icons/icons/deepin-auto-start.svg DESTINATION /usr/share/icons/hicolor/scalable/apps)
