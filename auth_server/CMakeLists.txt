cmake_minimum_required(VERSION 3.19)
project(auth_server LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets Network)

qt_standard_project_setup()

qt_add_executable(auth_server
    WIN32 MACOSX_BUNDLE
    main.cpp
    gui/MainWindow.cpp
    gui/MainWindow.h
    gui/MainWindow.ui
    server/Server.h server/Server.cpp
    auth/AuthController.h auth/AuthController.cpp
    session/ClientSession.h session/ClientSession.cpp
    db/UserStorage.cpp db/UserStorage.h
    validation/ValidationServer.h validation/ValidationServer.cpp
)

target_link_libraries(auth_server
    PRIVATE
        Qt::Core
        Qt::Widgets
        Qt::Network
)

target_include_directories(auth_server
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/server
        ${CMAKE_CURRENT_SOURCE_DIR}/auth
        ${CMAKE_CURRENT_SOURCE_DIR}/gui
        ${CMAKE_CURRENT_SOURCE_DIR}/session
        ${CMAKE_CURRENT_SOURCE_DIR}/db
        ${CMAKE_CURRENT_SOURCE_DIR}/validation
        ${CMAKE_SOURCE_DIR}/common
)

include(GNUInstallDirs)

install(TARGETS auth_server
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET auth_server
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
