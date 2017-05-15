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

	void init(NullLogContext& context, LogInfo& data) {
		UNUSED(context);
		UNUSED(data);
	}

	bool isEnabled() const {
		return false;
	}

    template<typename ... Args>
    void writeFormatted(const char*, Args ...) {
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

inline NullLogData& operator<<(NullLogData& data, bool v) {
    UNUSED(v);
    return data;
}

inline NullLogData& operator<<(NullLogData& data, const char* v) {
    UNUSED(v);
    return data;
}

template<size_t N>
inline NullLogData& operator<<(NullLogData& data, const char (&v)[N]) {
   UNUSED(v);
   return data;
}

inline NullLogData& operator<<(NullLogData& data, const std::string& v) {
    UNUSED(v);
    return data;
}

inline NullLogData& operator<<(NullLogData& data, float v) {
    UNUSED(v);
    return data;
}


// TODO : strangely, it seems like none of the types defined in "stdint.h" is equivalent to "long int" on a 32 bits platform
#if __WORDSIZE == 32
inline NullLogData& operator<<(NullLogData& data, long int v) {
    UNUSED(v);
    return data;
}
inline NullLogData& operator<<(NullLogData& data, unsigned long int v) {
    UNUSED(v);
    return data;
}
#endif

inline NullLogData& operator<<(NullLogData& data, double v) {
    UNUSED(v);
    return data;
}

inline NullLogData& operator<<(NullLogData& data, uint64_t v) {
    UNUSED(v);
    return data;
}

inline NullLogData& operator<<(NullLogData& data, int64_t v) {
    UNUSED(v);
    return data;
}

inline NullLogData& operator<<(NullLogData& data, uint32_t v) {
    UNUSED(v);
    return data;
}

inline NullLogData& operator<<(NullLogData& data, int32_t v) {
    UNUSED(v);
    return data;
}

inline NullLogData& operator<<(NullLogData& data, uint16_t v) {
    UNUSED(v);
    return data;
}

inline NullLogData& operator<<(NullLogData& data, int16_t v) {
    UNUSED(v);
    return data;
}

inline NullLogData& operator<<(NullLogData& data, uint8_t v) {
    UNUSED(v);
    return data;
}

inline NullLogData& operator<<(NullLogData& data, int8_t v) {
    UNUSED(v);
    return data;
}

}

