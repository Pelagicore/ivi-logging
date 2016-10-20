
cmake_minimum_required(VERSION 2.6)

set(PACKAGE ivi-logging)
PROJECT(${PACKAGE})

include(GNUInstallDirs)

SET( PROJECT_MAJOR_VERSION 1 )
SET( PROJECT_MINOR_VERSION 3 )
SET( PROJECT_PATCH_LEVEL 0 )

set(VERSION ${PROJECT_MAJOR_VERSION}.${PROJECT_MINOR_VERSION}.${PROJECT_PATCH_LEVEL})
set(PACKAGE_VERSION ${VERSION})

OPTION( ENABLE_DLT_BACKEND "Include DLT as default logging backend" OFF )
OPTION( ENABLE_CONSOLE_BACKEND "Include console as default logging backend" ON )

# Dependencies
FIND_PACKAGE(PkgConfig REQUIRED)
if (ENABLE_DLT_BACKEND)
	pkg_check_modules(DLT REQUIRED automotive-dlt)
	add_definitions(${DLT_CFLAGS_OTHER})
	include_directories(${DLT_INCLUDE_DIRS})
	link_directories(${DLT_LIBRARY_DIRS})

	set(ADDITIONAL_PKGCONFIG_DEPENDENCIES automotive-dlt)
	set(BACKEND_DEFINES "#define IVI_LOGGING_DLT_AVAILABLE")

	set(DLT_OR_NULL DltContextClass)

else()
	set(DLT_OR_NULL NullLogContext)
endif()

if (ENABLE_CONSOLE_BACKEND)
	set(CONSOLE_OR_NULL DefaultConsoleLogContext)
else()
	set(CONSOLE_OR_NULL NullLogContext)
endif()


set(CMAKE_CXX_STANDARD 11)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic -Wextra -Wno-variadic-macros -Wno-vla-extension -Wno-gnu-zero-variadic-macro-arguments")

include_directories(
	include
	${CMAKE_CURRENT_BINARY_DIR}
)

configure_file(include/ivi-logging-config.h.in ivi-logging-config.h @ONLY)

set(INCLUDE_FILES
	include/ivi-logging.h
	include/ivi-logging-types.h
	include/ivi-logging-stl.h
	include/ivi-logging-console.h
	include/ivi-logging-file.h
	include/ivi-logging-common.h
	include/ivi-logging-null.h
	include/ivi-logging-utils.h
	include/ivi-no-logging.h
	${CMAKE_CURRENT_BINARY_DIR}/ivi-logging-config.h
)

if (${DLT_FOUND})
	set(INCLUDE_FILES
		${INCLUDE_FILES}
		include/ivi-logging-dlt.h
	)
endif()


install(FILES ${INCLUDE_FILES} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PACKAGE})

set(LIB_NAME ${PACKAGE})

install(FILES IVILoggingConfig.cmake DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/IVILogging)

configure_file(ivi-logging.pc.in ivi-logging.pc @ONLY)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/ivi-logging.pc DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig)

set(DEVELOPMENT_INCLUDE_PATH " -I${CMAKE_CURRENT_SOURCE_DIR}/include -I${CMAKE_CURRENT_BINARY_DIR} #")
set(DEVELOPMENT_LIBRARY_PATH " -L${CMAKE_CURRENT_BINARY_DIR}/src #" )
configure_file(ivi-logging.pc.in ivi-logging-uninstalled.pc)
if(INSTALL_PKGCONFIG_UNINSTALLED_FILE)
    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/ivi-logging-uninstalled.pc DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig)
endif()


add_subdirectory (src)

add_subdirectory (test)

add_subdirectory (examples)
add_subdirectory (doc)

