#pragma once

#include "assert.h"

#include "dlt_user.h"

#include "ivi-logging-common.h"

namespace logging {

class DltLogData;

class DltContextClass : public DltContext {

public:
	typedef DltLogData LogDataType;

	DltContextClass() {
	}

	~DltContextClass() {
		dlt_unregister_context(this);
	}

	void setParentContext(LogContextCommon& context) {
		m_context = &context;
	}

	bool isEnabled(LogLevel logLevel);

	static DltLogLevelType getDLTLogLevel(LogLevel level) {
		DltLogLevelType v = DLT_LOG_DEFAULT;
		switch (level) {
		case LogLevel::Debug : v = DLT_LOG_DEBUG; break;
		case LogLevel::Info : v = DLT_LOG_INFO; break;
		case LogLevel::Warning : v = DLT_LOG_WARN; break;
		case LogLevel::Fatal : v = DLT_LOG_FATAL; break;
		case LogLevel::Error : v = DLT_LOG_ERROR; break;
		case LogLevel::Verbose : v = DLT_LOG_VERBOSE; break;
		case LogLevel::None : v = DLT_LOG_OFF; break;
		default : v = DLT_LOG_DEFAULT; break;
		}
		return v;
	}

	void registerContext() {
		if ( !isAppRegistered() ) {
			registerApp( s_pAppLogContext->m_id.c_str(), s_pAppLogContext->m_description.c_str() );
		}
		//		auto code =
		dlt_register_context( this, m_context->getID(), m_context->getDescription() );
		//		assert(code == 0);
		//		m_isInitialized = true;
		//	}

	}

	static bool& isAppRegistered() {
		static bool m_appRegistered = false;
		return m_appRegistered;
	}

	/**
	 * Register the application.
	 */
	static void registerApp(const char* id, const char* description) {
		pid_t pid = getpid();
		char descriptionWithPID[1024];
		snprintf(descriptionWithPID, sizeof(descriptionWithPID), "PID:%i / %s", pid, description);

		auto dltCode = dlt_register_app(id, descriptionWithPID);

		// TODO : The piece of code below would be useful if the DLT library didn't always return 0
		if (dltCode != 0 ) {
			char pidAsHexString[5];
			snprintf(pidAsHexString, sizeof(pidAsHexString), "%X", pid);
			dltCode = dlt_register_app(pidAsHexString, descriptionWithPID);
		}
		isAppRegistered() = true;
	}

private:
	LogContextCommon* m_context = nullptr;

};


class DltLogData : public DltContextData {

public:
	typedef DltContextClass ContextType;

	DltLogData() {
	}

	void setEnableSourceCodeLocationInfo(bool enabled) {
		m_enableSourceCodeLocationInfo = enabled;
	}

	void init(DltContextClass& context, LogDataCommon& data) {
		m_data = &data;
		m_context = &context;
		auto dltLogLevel = m_context->getDLTLogLevel(m_data->getLogLevel());

#ifdef DLT_2_10
		m_enabled = ( (m_context)->log_level_ptr && ( (dltLogLevel) <= (int)*( (m_context)->log_level_ptr ) ) &&
			      ( (dltLogLevel) != 0 ) ); // TODO: get that expression from the DLT itself

		if (m_enabled)
			dlt_user_log_write_start(m_context, this, dltLogLevel);
#elif DLT_2_9
		m_enabled = dlt_user_log_write_start(m_context, this, dltLogLevel);
#else
#error "Unsupported DLT version"
#endif
	}

	virtual ~DltLogData() {
		if (m_enabled) {
			if (m_enableSourceCodeLocationInfo) {
				if (m_data->getFileName() != nullptr) dlt_user_log_write_utf8_string(this, m_data->getFileName());
				if (m_data->getLineNumber() != -1) dlt_user_log_write_uint32(this, m_data->getLineNumber());
				if (m_data->getPrettyFunction() != nullptr) dlt_user_log_write_utf8_string(this,
												     m_data->getPrettyFunction());
			}

			//			auto r =
			dlt_user_log_write_finish(this);
			//			assert(r==0);
		}
	}

	bool isEnabled() {
		return m_enabled;
	}

	//	void writeFormatString(const char* v) {
	//		dlt_user_log_write_utf8_string(this, v);
	//	}

	template<typename ... Args>
	void writeFormatted(const char* format, Args ... args) {
		if (m_enabled) {
			char b[65536];

#pragma GCC diagnostic push
			// Make sure GCC does not complain about not being able to check the format string since it is no literal string
#pragma GCC diagnostic ignored "-Wformat-security"
			snprintf(b, sizeof(b), format, args ...);
#pragma GCC diagnostic pop

			dlt_user_log_write_utf8_string(this, b);
		}
	}

private:
	DltContextClass* m_context = nullptr;
	LogDataCommon* m_data = nullptr;
	bool m_enableSourceCodeLocationInfo = true; // todo mb: only for testing. false;
	bool m_enabled = false;

};

inline bool DltContextClass::isEnabled(LogLevel logLevel) {
#ifdef DLT_2_10
	auto dltLogLevel = getDLTLogLevel(logLevel);
	return ( (this)->log_level_ptr && ( (dltLogLevel) <= (int)*( (this)->log_level_ptr ) ) &&
		 ( (dltLogLevel) != 0 ) );    // TODO: get that expression from the DLT itself
#elif DLT_2_9
	DltContextData d;
	return dlt_user_log_write_start( this, &d, getDLTLogLevel(logLevel) );
#endif
}

inline DltLogData& operator<<(DltLogData& data, bool v) {
	dlt_user_log_write_bool(&data, v);
	return data;
}

inline DltLogData& operator<<(DltLogData& data, const char* v) {
	dlt_user_log_write_utf8_string(&data, v);
	return data;
}

inline DltLogData& operator<<(DltLogData& data, float f) {
	// we assume a float is 32 bits
	dlt_user_log_write_float32(&data, f);
	return data;
}


// TODO : strangely, it seems like none of the types defined in "stding.h" is equivalent to "long int" on a 32 bits platform
#if __WORDSIZE == 32
inline DltLogData& operator<<(DltLogData& data, long int v) {
	dlt_user_log_write_int32(&data, v);
	return data;
}
#endif

inline DltLogData& operator<<(DltLogData& data, double f) {
	// we assume a double is 64 bits
	dlt_user_log_write_float64(&data, f);
	return data;
}

inline DltLogData& operator<<(DltLogData& data, uint64_t v) {
	dlt_user_log_write_uint64(&data, v);
	return data;
}

inline DltLogData& operator<<(DltLogData& data, int64_t v) {
	dlt_user_log_write_int64(&data, v);
	return data;
}

inline DltLogData& operator<<(DltLogData& data, uint32_t v) {
	dlt_user_log_write_uint32(&data, v);
	return data;
}

inline DltLogData& operator<<(DltLogData& data, int32_t v) {
	dlt_user_log_write_int32(&data, v);
	return data;
}

inline DltLogData& operator<<(DltLogData& data, uint16_t v) {
	dlt_user_log_write_uint16(&data, v);
	return data;
}

inline DltLogData& operator<<(DltLogData& data, int16_t v) {
	dlt_user_log_write_int16(&data, v);
	return data;
}

inline DltLogData& operator<<(DltLogData& data, uint8_t v) {
	dlt_user_log_write_uint8(&data, v);
	return data;
}

inline DltLogData& operator<<(DltLogData& data, int8_t v) {
	dlt_user_log_write_int8(&data, v);
	return data;
}

}
