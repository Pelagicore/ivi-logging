#pragma once

#include "ivi-logging.h"

namespace logging {

/**
 * Generate a log with "fatal" severity
 */
#define log_fatal(args ...)

/**
 * Generate a log with "error" severity
 */
#define log_error(args ...)

/**
 * Generate a log with "verbose" severity
 */
#define log_verbose(args ...)

/**
 * Generate a log with "info" severity
 */
#define log_info(args ...)

/**
 * Generate a log with "warning" severity
 */
#define log_warn(args ...)
#define log_warning(args ...)

/**
 * Generate a log with "debug" severity
 */
#define log_debug(args ...)

/**
 * Defines the identifiers of an application. This macro should be used at one place in every application.
 */
#define LOG_DEFINE_APP_IDS(appID, appDescription)

/**
 * Create a LogContext with the given ID (4 characters in case of DLT support) and description
 */
#define LOG_DECLARE_CONTEXT(contextName, contextShortID, contextDescription)
/**
 * Create a new context and define is as default context for the current scope
 */
#define LOG_DECLARE_DEFAULT_CONTEXT(context, contextID, contextDescription)

/**
 * Import the given context, which should be exported by another module
 */
#define LOG_IMPORT_CONTEXT(contextName)

/**
 * Set the given context as default for the current scope
 */
#define LOG_SET_DEFAULT_CONTEXT(context)
/**
 * Import the given context and set it as default for the current scope
 */
#define LOG_IMPORT_DEFAULT_CONTEXT(context)

/**
 * Set the given context as default for the current class
 */
#define LOG_SET_CLASS_CONTEXT(context)

/**
 *
 */
#define LOG_DECLARE_DEFAULT_LOCAL_CONTEXT(contextShortID, contextDescription)

/**
 * Set a new context as default context for a class. To be used inside the class definition.
 */
#define LOG_DECLARE_CLASS_CONTEXT(contextShortID, contextDescription)

}
