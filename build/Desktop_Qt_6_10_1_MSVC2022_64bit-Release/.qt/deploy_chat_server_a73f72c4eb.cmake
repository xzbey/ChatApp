include("D:/qtprojects/ChatApp/build/Desktop_Qt_6_10_1_MSVC2022_64bit-Release/.qt/QtDeploySupport.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/chat_server-plugins.cmake" OPTIONAL)
set(__QT_DEPLOY_I18N_CATALOGS "qtbase;qtdeclarative;qtdeclarative;qtdeclarative;qtdeclarative;qtdeclarative;qtdeclarative;qtdeclarative")

qt6_deploy_runtime_dependencies(
    EXECUTABLE "D:/qtprojects/ChatApp/build/Desktop_Qt_6_10_1_MSVC2022_64bit-Release/chat_server/chat_server.exe"
    GENERATE_QT_CONF
)
