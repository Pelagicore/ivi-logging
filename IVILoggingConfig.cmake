FIND_PACKAGE(PkgConfig REQUIRED)
pkg_check_modules(IVILOGGING REQUIRED ivi-logging)
add_definitions(${IVILOGGING_CFLAGS})
link_directories(${IVILOGGING_LIBRARY_DIRS})
