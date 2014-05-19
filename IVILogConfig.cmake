FIND_PACKAGE(PkgConfig REQUIRED)
pkg_check_modules(IVILOG REQUIRED ivi-log)
add_definitions(${IVILOG_CFLAGS})
link_directories(${IVILOG_LIBRARY_DIRS})
