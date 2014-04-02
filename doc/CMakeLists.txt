
find_package(Doxygen)
if (DOXYGEN_FOUND STREQUAL "NO")
    message("Doxygen not found. Please get a copy http://www.doxygen.org")
else()

	# prepare doxygen configuration file
	set(DOXYGEN_SRCDIR ${PROJECT_SOURCE_DIR})
	set(OUTDIR ${PROJECT_BINARY_DIR})
	set(DOXYGEN_EXAMPLES_PATH ${PROJECT_SOURCE_DIR}/examples)
	set(DOXYGEN_PROJECT_NAME ${PACKAGE})
	configure_file(${CMAKE_CURRENT_SOURCE_DIR}/../pelagicore.doxygen.cfg.in ${PROJECT_BINARY_DIR}/doc/doxygen.cfg)
	
	# add doxygen as target
	add_custom_command(OUTPUT ${PROJECT_BINARY_DIR}/doc/doxygen COMMAND ${DOXYGEN_EXECUTABLE} ARGS ${PROJECT_BINARY_DIR}/doc/doxygen.cfg)
	
	# cleanup $build/doc/doxygen on "make clean"
	set_property(DIRECTORY APPEND PROPERTY
		     ADDITIONAL_MAKE_CLEAN_FILES doc/doxygen)
	
	add_custom_target(
		documentation ALL
		DEPENDS
		${PROJECT_BINARY_DIR}/doc/doxygen
	)
	
	# install HTML API documentation and manual pages
	set(DOC_PATH "share/doc/${PACKAGE}")
	
	install(DIRECTORY  ${PROJECT_BINARY_DIR}/doc/doxygen
	         DESTINATION ${DOC_PATH}
	)

endif()
