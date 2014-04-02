#pragma once

// Enable verbose logs for the console output, which are disabled by default
#define LOG_CONSOLE_SEVERITY LogLevel::Verbose

#include "pelagicore-log.h"

// Import the application's contexts so that they can be used in any file which includes this header
LOG_IMPORT_CONTEXT(mainContext);
LOG_IMPORT_CONTEXT(anotherContext);

void myFunction2();
