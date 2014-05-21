/**
 * This file define how to output standard types to the logging system
 */
#pragma once

#include "ivi-logging-common.h"
#include "ivi-logging.h"
#include <vector>
#include <string>
#include <type_traits>

namespace logging {

template<typename ElementType, class LogDataType = logging::LogDataCommon, typename =
		 typename std::enable_if<std::is_base_of<logging::LogDataCommon, LogDataType>::value>::type>
LogDataType& operator<<(LogDataType& log, const std::vector<ElementType>& v) {
	log << " [ ";
	for (auto& element : v) {
		log << element;
		log << ", ";
	}
	log << " ] ";
	return log;
}

template<typename LogDataType, typename =
		 typename std::enable_if<std::is_base_of<logging::LogDataCommon, LogDataType>::value>::type>
LogDataType& operator<<(LogDataType& log, const void* v) {
	log << (size_t)v;
	return log;
}

template<typename LogDataType, typename =
		 typename std::enable_if<std::is_base_of<logging::LogDataCommon, LogDataType>::value>::type>
LogDataType& operator<<(LogDataType& log, const std::string& s) {
	log << s.c_str();
	return log;
}

}
