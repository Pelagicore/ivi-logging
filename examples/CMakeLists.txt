
INCLUDE_DIRECTORIES( ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR} )

add_executable( pelagicore-logging-example
	exampleLoggingMain.cpp
	exampleLoggingOtherFile.cpp
)

TARGET_LINK_LIBRARIES( pelagicore-logging-example
	pelagicore-logging
)

install(TARGETS pelagicore-logging-example
		DESTINATION bin)
