set(BIN_NAME deepin-pc-manager)
# 设置包含当前目录，建议头文件的包含写全路径
set(CMAKE_INCLUDE_CURRENT_DIR ON)


# 若目标库安装了.cmake文件，则可以直接使用find_package
find_package(PkgConfig REQUIRED)
pkg_check_modules(GSettings REQUIRED IMPORTED_TARGET gsettings-qt)
pkg_check_modules(PolkitQt REQUIRED IMPORTED_TARGET polkit-qt5-1)
pkg_check_modules(LibParted REQUIRED IMPORTED_TARGET libparted)

find_package(QT NAMES Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Sql Gui Widgets DBus Concurrent)
find_package(Dtk REQUIRED COMPONENTS Widget)

if (NOT (${CMAKE_BUILD_TYPE} STREQUAL "Debug"))
  execute_process(COMMAND bash "translate_generation.sh"
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  )

  file(GLOB APP_QM_FILES "translations/*.qm")
  install(FILES ${APP_QM_FILES} DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/${BIN_NAME}/translations)
endif()

# 建议对不同的模块进行分类，可使用通配符，可指定具名文件
file(GLOB_RECURSE SRCS
    "*.xml"
    "src/*.h"
    "src/*.cpp"
    "configs/*.json"
    "include/*.h"
)
# 生成可执行文件
add_executable(${BIN_NAME}
    ${SRCS}
    defender.qrc
    src/window/icons/defender_icons.qrc
)

# Qt 从5.15版本开始，可以直接使用Qt::Core，而不需要加版本号，但为了兼容性，把版本号加上为好
target_include_directories(${BIN_NAME} PUBLIC
  Qt${QT_VERSION_MAJOR}::Core
  Qt${QT_VERSION_MAJOR}::Sql
  Qt${QT_VERSION_MAJOR}::Gui
  Qt${QT_VERSION_MAJOR}::Widgets
  Qt${QT_VERSION_MAJOR}::Concurrent
  Dtk::Widget
  PkgConfig::GSettings
  PkgConfig::PolkitQt
  PkgConfig::LibParted
  /usr/include/deepin-pc-manager/
  src/
)
target_link_libraries(${BIN_NAME} PRIVATE
  Qt${QT_VERSION_MAJOR}::Core
  Qt${QT_VERSION_MAJOR}::Sql
  Qt${QT_VERSION_MAJOR}::Gui
  Qt${QT_VERSION_MAJOR}::Widgets
  Qt${QT_VERSION_MAJOR}::Concurrent
  Dtk::Widget
  PkgConfig::GSettings
  PkgConfig::PolkitQt
  PkgConfig::LibParted
)

# 指定安装目录，一般常用有3个： TARGETS（编译出来的目标二进制）、FILES（指定路径的文件，通常是配置文件或服务文件）、DIRECTORY（一般是配置文件较多时使用）。
install(TARGETS ${BIN_NAME} DESTINATION ${CMAKE_INSTALL_BINDIR})

install(FILES ./deepin-pc-manager.desktop DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/applications)
install(FILES ./com.deepin.deepin-pc-manager.policy DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/polkit-1/actions)
install(FILES ./com.deepin.pc.manager.service DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/dbus-1/services)
install(FILES ./src/window/icons/icons/deepin-pc-manager.svg DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/icons/hicolor/scalable/apps)
install(FILES ./configs/org.deepin.dde.deepin-pc-manager.json DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/dsg/configs/org.deepin.dde.deepin-pc-manager/)
install(FILES ./cleanerwhite.json DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/deepin-pc-manager/)

file(GLOB HEADERS "include/*")
# dev files
set(INCLUDE_INSTALL_DIR ${CMAKE_INSTALL_INCLUDEDIR}/deepin-pc-manager)
install(DIRECTORY ${HEADERS} DESTINATION ${INCLUDE_INSTALL_DIR})
