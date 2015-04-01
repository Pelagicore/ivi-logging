
INCLUDE_DIRECTORIES( ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR} )

add_executable( logging-example-file
	exampleLoggingFile.cpp
)

TARGET_LINK_LIBRARIES( logging-example-file
	${LIB_NAME}
)

install(TARGETS logging-example-file
		DESTINATION ${CMAKE_INSTALL_BINDIR})
