
cmake_minimum_required(VERSION 2.6)

set(PACKAGE pelagicore-logging)
PROJECT(${PACKAGE})

set(prefix ${CMAKE_INSTALL_PREFIX})
set(libdir ${CMAKE_INSTALL_PREFIX}/lib)
set(includedir ${CMAKE_INSTALL_PREFIX}/include)

# Dependencies
FIND_PACKAGE(PkgConfig REQUIRED)

if (NOT DEFINED DISABLE_DLT)
	pkg_check_modules(DLT automotive-dlt)
	if (${DLT_FOUND})
		add_definitions(${DLT_CFLAGS})
		add_definitions(-DENABLE_DLT_LOGGING)
		set (ADDITIONAL_PKGCONFIG_DEPENDENCIES automotive-dlt)
		set (ADDITIONAL_PKGCONFIG_CFLAGS -DENABLE_DLT_LOGGING)

		set (LOGGING_LIBRARY_DIRS ${DLT_LIBRARY_DIRS} PARENT_SCOPE)

#		set(DLT_LIBRARY_FILES src/log-dlt.cpp) 

		link_directories(
			${DLT_LIBRARY_DIRS}
		)

	endif()
endif()

set(PACKAGE_VERSION 1.0.0)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++11")

include_directories(
	include
)

set(INCLUDE_FILES
	include/plog.h
	include/log-types.h
	include/log-console.h
	include/log-file.h
	include/log-common.h
	include/ByteArray.h
)

set(INCLUDE_FILES
	${INCLUDE_FILES}
	qt/pelagicore-log-qt.h
)


if (${DLT_FOUND})
	set(INCLUDE_FILES
		${INCLUDE_FILES}
		include/log-dlt.h
	)
endif()


install(FILES ${INCLUDE_FILES} DESTINATION include/pelagicore-logging)


add_library( pelagicore-logging SHARED
	src/log.cpp
	${DLT_LIBRARY_FILES}
)

set_target_properties(pelagicore-logging PROPERTIES VERSION ${VERSION} SOVERSION ${${PROJECT_NAME}_MAJOR_VERSION})

TARGET_LINK_LIBRARIES( pelagicore-logging
   ${DLT_LIBRARIES}
   ${CPP_LIBS}
)

INSTALL(TARGETS pelagicore-logging
  DESTINATION lib
)

install(FILES PelagicoreLoggingConfig.cmake DESTINATION lib/cmake/PelagicoreLogging)

configure_file(pelagicore-logging.pc.in pelagicore-logging.pc)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/pelagicore-logging.pc DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/pkgconfig)


add_subdirectory (examples)
add_subdirectory (doc)

