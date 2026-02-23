add_library(common STATIC
    ValidationProtocol.h)

target_sources(common
    PRIVATE
        AuthProtocol.h
        ChatProtocol.h
	Utils.h
)

target_include_directories(common
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(common
    PUBLIC
        Qt6::Core
)
