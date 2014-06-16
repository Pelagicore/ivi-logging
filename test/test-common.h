#pragma once

#include "ivi-logging.h"
#include "ivi-logging-console.h"

#ifdef ENABLE_DLT_LOGGING
#include "ivi-logging-dlt.h"
#endif

typedef logging::LogContextT<
	logging::TypeSet<logging::ConsoleLogContext
#ifdef ENABLE_DLT_LOGGING
			 , logging::DltContextClass
#endif
			 >,
	logging::TypeSet<logging::ConsoleLogContext::LogDataType
#ifdef ENABLE_DLT_LOGGING
			 , logging::DltContextClass::LogDataType
#endif
			 > > LogContextWithConsolePlusDLTIfAvailable;
