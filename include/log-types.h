/**
 * This file define how to output standard types to the logging system
 */
#pragma once

#include "pelagicore-log.h"
#include <vector>
#include <string>

namespace pelagicore {

//template<typename T>
//inline LogData& operator<<(LogData& log, const T* v) {
//	log << pointerToString(v);
//	return log;
//}

template<typename Type>
LogData& operator<<(LogData& log, const Type& v) {
#ifdef ENABLE_DLT_LOGGING
	log.m_dltLog << v;
#endif
#ifdef ENABLE_CONSOLE_LOGGING
	log.m_consoleLog << v;
#endif

	return log;
}


template<typename ElementType>
LogData& operator<<(LogData& log, const std::vector<ElementType>& v) {
	log << " [ ";
	for (auto& element : v) {
		log << element;
		log << ", ";
	}
	log << " ] ";
	return log;
}

inline LogData& operator<<(LogData& log, const void* v) {
	log << (size_t)v;
	return log;
}


inline LogData& operator<<(LogData& log, const std::string& s) {
	log << s.c_str();
	return log;
}

}
