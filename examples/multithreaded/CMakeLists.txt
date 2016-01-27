
INCLUDE_DIRECTORIES( ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR} )

add_executable( logging-multithreaded
	exampleLoggingMultithreaded.cpp
)

TARGET_LINK_LIBRARIES( logging-multithreaded
	${LIB_NAME}
)

install(TARGETS logging-multithreaded
		DESTINATION ${CMAKE_INSTALL_BINDIR})
