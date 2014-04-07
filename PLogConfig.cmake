
FIND_PACKAGE(PkgConfig REQUIRED)
pkg_check_modules(PLOG REQUIRED plog)
add_definitions(${PLOG_CFLAGS})
link_directories(${PLOG_LIBRARY_DIRS})

set(LOGGING_PKG_CONFIG_DEPENDENCIES "plog")
