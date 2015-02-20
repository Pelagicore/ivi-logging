/**
 * This file define how to output standard types to the logging system
 */
#pragma once

#include "ivi-logging-common.h"
#include <vector>
#include <map>
#include <unordered_map>
#include <string>
#include <exception>
#include <type_traits>
#include "ivi-logging-stl.h"

namespace logging {

static constexpr const char* NULL_POINTER_STRING = "nullptr";

template<typename LogDataType, typename =
         typename std::enable_if<std::is_base_of<logging::LogData, LogDataType>::value>::type>
LogDataType& operator<<(LogDataType& log, const StringBuilder& b) {
    log << b.str();
    return log;
}

}


