
INCLUDE_DIRECTORIES( ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR} )

add_executable( logging-example
	exampleLoggingMain.cpp
	exampleLoggingOtherFile.cpp
)

TARGET_LINK_LIBRARIES( logging-example
	${LIB_NAME}
)

install(TARGETS logging-example
		DESTINATION bin)
