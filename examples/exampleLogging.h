#pragma once

// Enable verbose logs for the console output, which are disabled by default
#define LOG_CONSOLE_SEVERITY LogLevel::Verbose

#include "plog.h"

#ifdef ENABLE_DLT_LOGGING
#include "log-dlt.h"
#endif

#ifdef ENABLE_CONSOLE_LOGGING
#include "log-console.h"
#endif


namespace pelagicore {
typedef pelagicore::LogContextT<Input<pelagicore::ConsoleLogContext,
				      pelagicore::DltContextClass>, Output<ConsoleLogData, DltLogData> > LogContext;
typedef pelagicore::LogContextT<Input<pelagicore::DltContextClass>, Output<DltLogData> > LogContext9;
typedef pelagicore::LogContextT<Input<pelagicore::DltContextClass,
				      pelagicore::DltContextClass>, Output<DltLogData, DltLogData> > LogContext8;
typedef pelagicore::LogContextT<Input<pelagicore::ConsoleLogContext
				      >, Output<ConsoleLogData> > LogContext2;
}

// Import the application's contexts so that they can be used in any file which includes this header
LOG_IMPORT_CONTEXT(mainContext);
LOG_IMPORT_CONTEXT(anotherContext);

void myFunction2();
