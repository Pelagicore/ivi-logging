#pragma once

#include "ivi-logging.h"

#ifdef ENABLE_DLT_LOGGING
#include "ivi-logging-dlt.h"
#endif

#include "ivi-logging-console.h"
#include "ivi-logging-file.h"

/**
 * We define our actual LogContext type, which will be used when calling the log macros
 */
typedef logging::LogContextT<
	logging::TypeSet<
		logging::ConsoleLogContext
#ifdef ENABLE_DLT_LOGGING
		, logging::DltContextClass
#endif
		>, logging::TypeSet<
		logging::ConsoleLogContext::LogDataType
#ifdef ENABLE_DLT_LOGGING
		, logging::DltContextClass::LogDataType
#endif
		> > LogContext;


// Import the application's contexts so that they can be used in any file which includes this header
LOG_IMPORT_CONTEXT(mainContext);
LOG_IMPORT_CONTEXT(anotherContext);

void myFunction2();

inline void disableConsoleLogging() {
	logging::ConsoleLogContext::setGlobalLogLevel(logging::LogLevel::None);
}
