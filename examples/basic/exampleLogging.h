#pragma once

#include "logging-basic.h"

// Import the application's contexts so that they can be used in any file which includes this header
LOG_IMPORT_CONTEXT(mainContext);
LOG_IMPORT_CONTEXT(anotherContext);

void myFunction2();

inline void disableConsoleLogging() {
	logging::ConsoleLogContext::setGlobalLogLevel(logging::LogLevel::None);
}
