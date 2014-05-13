FIND_PACKAGE(PkgConfig REQUIRED)
pkg_check_modules(PLOG REQUIRED plog)
add_definitions(${PLOG_CFLAGS})
link_directories(${PLOG_LIBRARY_DIRS})
