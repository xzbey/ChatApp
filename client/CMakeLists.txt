cmake_minimum_required(VERSION 3.16)

project(client VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Quick Widgets)

qt_standard_project_setup()

qt_add_executable(appclient
    main.cpp
)

qt_add_qml_module(appclient
    URI client
    VERSION 1.0
    QML_FILES
        qml/Auth.qml
        qml/Chat.qml
        common/Utils.js
    SOURCES
        auth/AuthService.h
        auth/AuthService.cpp
        app/ApplicationController.h
        app/ApplicationController.cpp
        chat/ChatService.h
        chat/ChatService.cpp
        network/Client.h
        network/Client.cpp
)

set_target_properties(appclient PROPERTIES
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appclient
    PRIVATE
    Qt6::Quick
    Qt6::Widgets
)

target_include_directories(appclient
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/app
        ${CMAKE_CURRENT_SOURCE_DIR}/auth
        ${CMAKE_CURRENT_SOURCE_DIR}/chat
        ${CMAKE_CURRENT_SOURCE_DIR}/common
        ${CMAKE_CURRENT_SOURCE_DIR}/network
        ${CMAKE_CURRENT_SOURCE_DIR}/qml
        ${CMAKE_SOURCE_DIR}/common
)

include(GNUInstallDirs)
install(TARGETS appclient
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
