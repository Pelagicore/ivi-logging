
cmake_minimum_required(VERSION 2.6)

set(PACKAGE ivi-log)
PROJECT(${PACKAGE})

SET( ${PROJECT_NAME}_MAJOR_VERSION 1 )
SET( ${PROJECT_NAME}_MINOR_VERSION 0 )
SET( ${PROJECT_NAME}_PATCH_LEVEL 0 )

set(VERSION ${${PROJECT_NAME}_MAJOR_VERSION}.${${PROJECT_NAME}_MINOR_VERSION}.${${PROJECT_NAME}_PATCH_LEVEL})
set(PACKAGE_VERSION ${VERSION})

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

		link_directories(
			${DLT_LIBRARY_DIRS}
		)

	endif()
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++11")

include_directories(
	include
)

set(INCLUDE_FILES
	include/ivi-log.h
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


install(FILES ${INCLUDE_FILES} DESTINATION include/${PACKAGE})

set(LIB_NAME ${PACKAGE})

add_library( ${LIB_NAME} SHARED
	src/log.cpp
	${DLT_LIBRARY_FILES}
)

set_target_properties(${LIB_NAME} PROPERTIES VERSION ${VERSION} SOVERSION ${${PROJECT_NAME}_MAJOR_VERSION})

TARGET_LINK_LIBRARIES( ${LIB_NAME}
   ${DLT_LIBRARIES}
   ${CPP_LIBS}
)

INSTALL(TARGETS ${LIB_NAME}
  DESTINATION lib
)

install(FILES IVILogConfig.cmake DESTINATION lib/cmake/IVILog)

configure_file(ivi-log.pc.in ivi-log.pc)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/ivi-log.pc DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/pkgconfig)


add_subdirectory (examples)
add_subdirectory (doc)

