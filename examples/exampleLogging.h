#pragma once

#include "ivi-logging.h"

/**
 * We define our actual LogContext type, which defines the list of backends to use for the logging. Here, we simply reuse the default list of backends
 */
typedef logging::DefaultLogContext LogContext;

// Import the application's contexts so that they can be used in any file which includes this header
LOG_IMPORT_CONTEXT(mainContext);
LOG_IMPORT_CONTEXT(anotherContext);

void myFunction2();

inline void disableConsoleLogging() {
	logging::ConsoleLogContext::setGlobalLogLevel(logging::LogLevel::None);
}
