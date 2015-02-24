#pragma once

#include "ivi-logging.h"
#include "ivi-logging-file.h"

// We reuse the default configuration of ivi-logging, but we add our own file logging backend
typedef logging::DefaultLogContext::Extension<logging::FileLogContext, logging::FileLogContext::LogDataType> LogContext;
