#pragma once

#include "plog.h"

#ifdef ENABLE_DLT_LOGGING
#include "log-dlt.h"
#endif

#include "log-console.h"
#include "log-file.h"

/**
 * We define our actual LogContext type, which will be used when calling the log macros
 */
typedef logging::LogContextT<
	logging::TypeSet<
		logging::ConsoleLogContext,
#ifdef ENABLE_DLT_LOGGING
		logging::DltContextClass
#endif
		>, logging::TypeSet<
		logging::ConsoleLogContext::LogDataType,
#ifdef ENABLE_DLT_LOGGING
		logging::DltContextClass::LogDataType
#endif
		> > LogContext;


// Import the application's contexts so that they can be used in any file which includes this header
LOG_IMPORT_CONTEXT(mainContext);
LOG_IMPORT_CONTEXT(anotherContext);

void myFunction2();
