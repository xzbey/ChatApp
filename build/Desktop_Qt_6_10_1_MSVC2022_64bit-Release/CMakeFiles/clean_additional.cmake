# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Release")
  file(REMOVE_RECURSE
  "auth_server\\CMakeFiles\\auth_server_autogen.dir\\AutogenUsed.txt"
  "auth_server\\CMakeFiles\\auth_server_autogen.dir\\ParseCache.txt"
  "auth_server\\auth_server_autogen"
  "chat_server\\CMakeFiles\\chat_server_autogen.dir\\AutogenUsed.txt"
  "chat_server\\CMakeFiles\\chat_server_autogen.dir\\ParseCache.txt"
  "chat_server\\chat_server_autogen"
  "client\\CMakeFiles\\appclient_autogen.dir\\AutogenUsed.txt"
  "client\\CMakeFiles\\appclient_autogen.dir\\ParseCache.txt"
  "client\\appclient_autogen"
  "common\\CMakeFiles\\common_autogen.dir\\AutogenUsed.txt"
  "common\\CMakeFiles\\common_autogen.dir\\ParseCache.txt"
  "common\\common_autogen"
  )
endif()
