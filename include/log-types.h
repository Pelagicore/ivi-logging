/**
 * This file define how to output standard types to the logging system
 */
#pragma once

#include "log-common.h"
#include <vector>
#include <string>

namespace pelagicore {

template<typename ElementType>
typename LogContext::LogData& operator<<(LogContext::LogData& log, const std::vector<ElementType>& v) {
	log << " [ ";
	for (auto& element : v) {
		log << element;
		log << ", ";
	}
	log << " ] ";
	return log;
}

inline LogContext::LogData& operator<<(LogContext::LogData& log, const void* v) {
	log << (size_t)v;
	return log;
}

inline typename LogContext::LogData& operator<<(LogContext::LogData& log, const std::string& s) {
	log << s.c_str();
	return log;
}

}
