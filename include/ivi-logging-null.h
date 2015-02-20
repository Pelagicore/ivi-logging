#pragma once

#include "ivi-logging-common.h"

namespace logging {

class NullLogContext;

/**
 * This class provides the same interface as other LogData classes, but actually does nothing with the objects which are streamed to it.
 * It can be useful to disable some logging.
 */
class NullLogData : public LogData {

public:

	template<typename DataType>
	NullLogData& operator<<(const DataType& v) {
		UNUSED(v);
		return *this;
	}

	void init(NullLogContext& context, LogInfo& data) {
		UNUSED(context);
		UNUSED(data);
	}

	bool isEnabled() const {
		return false;
	}

    template<typename ... Args>
    void writeFormatted(const char* format, Args ... args) {
    }

};

class NullLogContext {

public:

	typedef NullLogData LogDataType;

	void setParentContext(LogContextCommon& context) {
		UNUSED(context);
	}

	bool isEnabled(LogLevel logLevel) {
		UNUSED(logLevel);
		return false;
	}

	void registerContext() {
	}

};



}

