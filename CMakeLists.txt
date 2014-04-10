
cmake_minimum_required(VERSION 2.6)

set(PACKAGE plog)
PROJECT(${PACKAGE})

SET( ${PROJECT_NAME}_MAJOR_VERSION 0 )
SET( ${PROJECT_NAME}_MINOR_VERSION 9 )
SET( ${PROJECT_NAME}_PATCH_LEVEL 0 )

set(VERSION ${${PROJECT_NAME}_MAJOR_VERSION}.${${PROJECT_NAME}_MINOR_VERSION}.${${PROJECT_NAME}_PATCH_LEVEL})

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
	include/log-null.h
	include/ByteArray.h
)

set(INCLUDE_FILES
	${INCLUDE_FILES}
	qt/log-qt.h
)


if (${DLT_FOUND})
	set(INCLUDE_FILES
		${INCLUDE_FILES}
		include/log-dlt.h
	)
endif()


install(FILES ${INCLUDE_FILES} DESTINATION include/plog)


add_library( plog SHARED
	src/log.cpp
	${DLT_LIBRARY_FILES}
)

set_target_properties(plog PROPERTIES VERSION ${VERSION} SOVERSION ${${PROJECT_NAME}_MAJOR_VERSION})

TARGET_LINK_LIBRARIES( plog
   ${DLT_LIBRARIES}
   ${CPP_LIBS}
)

INSTALL(TARGETS plog
  DESTINATION lib
)

install(FILES PLogConfig.cmake DESTINATION lib/cmake/PLog)

configure_file(plog.pc.in plog.pc)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/plog.pc DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/pkgconfig)


add_subdirectory (examples)
add_subdirectory (doc)

