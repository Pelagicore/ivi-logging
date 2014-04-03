#pragma once

#include <string>
#include <functional>
#include "string.h"

#include "log-common.h"

#ifdef ENABLE_DLT_LOGGING
#include "log-dlt.h"
#endif

#ifdef ENABLE_CONSOLE_LOGGING
#include "log-console.h"
#endif

namespace pelagicore {

inline std::string pointerToString(const void* p) {
	char buffer[32];
	sprintf(buffer, "0x%zX", (size_t) p);
	return buffer;
}

int getThreadID();

struct DummyClass {
};

/**
 * A logging context
 */
class LogContext : private DummyClass
#ifdef ENABLE_DLT_LOGGING
	, public DltContextClass
#endif

#ifdef ENABLE_CONSOLE_LOGGING
	, public ConsoleLogContext
#endif
{
public:
	LogContext(const char* id, const char* contextDescription) : DummyClass()
#ifdef ENABLE_DLT_LOGGING
		, DltContextClass(id, contextDescription)
#endif

#ifdef ENABLE_CONSOLE_LOGGING
		, ConsoleLogContext(id, contextDescription)
#endif

	{
		if (strlen(id) > 4)
			fprintf(
				stderr,
				LOGGING_WARNING_OUTPUT_PREFIX
				"Log IDs should not be longer than 4 characters to be compatible with the DLT : %s\n",
				id);
	}

	~LogContext() {
	}

	void checkContext() {
		if (!m_bRegistered) {
			registerDefaultAPPIDSIfNeeded();
			if (!s_pAppLogContext->consoleRegistered) {
				//			fprintf(stdout, LOGGING_WARNING_OUTPUT_PREFIX "Initializing logging, appID:\"%s\", description:\"%s\"\n", s_pAppLogContext->m_id,					s_pAppLogContext->m_description);
				s_pAppLogContext->consoleRegistered = true;
			}
	#ifdef ENABLE_DLT_LOGGING
			DltContextClass::registerContext();
	#endif
			m_bRegistered = true;
		}

	}

	bool isEnabled(LogLevel level) {

		checkContext();

		bool enabled = false;

#ifdef ENABLE_CONSOLE_LOGGING
		enabled |= ConsoleLogContext::isEnabled(level);
#endif

#ifdef ENABLE_DLT_LOGGING
		enabled |= DltContextClass::isEnabled(level);
#endif

		return enabled;
	}

	bool m_bRegistered = false;
};

class LogData : private DummyClass {
public:
	LogData(LogContext& context, LogLevel level, const char* fileName, int lineNumber,
		const char* prettyFunction) : DummyClass()

#ifdef ENABLE_DLT_LOGGING
		, m_dltLog(context, level, fileName, lineNumber, prettyFunction)
#endif

#ifdef ENABLE_CONSOLE_LOGGING
		, m_consoleLog(context, level, fileName, lineNumber, prettyFunction)
#endif

	{
	}

	~LogData() {
	}

	LogData& writeFormatted() {
		return *this;
	}

	template<typename ... Args>
	LogData& writeFormatted(const char* format, Args ... args) {
#ifdef ENABLE_DLT_LOGGING
		m_dltLog.writeFormatted(format, args ...);
#endif
#ifdef ENABLE_CONSOLE_LOGGING
		m_consoleLog.writeFormatted(format, args ...);
#endif
		return *this;
	}

#ifdef ENABLE_DLT_LOGGING
	DltLogData m_dltLog;
#endif

#ifdef ENABLE_CONSOLE_LOGGING
	ConsoleLogData m_consoleLog;
#endif

};

#define buildFormatString(format) format

#define log_with_context(context, severity, args ...) \
	for (pelagicore::LogContext* dummy = &context; \
	     (dummy != \
	      nullptr) && dummy->isEnabled(severity); dummy = nullptr) pelagicore::LogData(*dummy, severity, __FILE__, __LINE__, \
											   __PRETTY_FUNCTION__)	\
		.writeFormatted(args)

#ifndef log_error

/**
 * Generate a log with "error" severity
 */
#define log_error(args ...) log_with_context(getDefaultContext(), pelagicore::LogLevel::Error, ## args)

/**
 * Generate a log with "verbose" severity
 */
#define log_verbose(args ...) log_with_context(getDefaultContext(), pelagicore::LogLevel::Verbose, ## args)

/**
 * Generate a log with "info" severity
 */
#define log_info(args ...) log_with_context(getDefaultContext(), pelagicore::LogLevel::Info, ## args)

/**
 * Generate a log with "warning" severity
 */
#define log_warn(args ...) log_with_context(getDefaultContext(), pelagicore::LogLevel::Warning, ## args)
#define log_warning(args ...) log_warn(args)

/**
 * Generate a log with "debug" severity
 */
#define log_debug(args ...) log_with_context(getDefaultContext(), pelagicore::LogLevel::Debug, ## args)

/**
 * Defines the identifiers of an application. This macro should be used exactly at one place in every application
 */
#define LOG_DEFINE_APP_IDS(appID, appDescription) \
	pelagicore::AppLogContext s_appLogContext(appID, appDescription);

/**
 * Create a LogContext with the given ID (4 characters in case of DLT support) and description
 */
#define LOG_DECLARE_CONTEXT(contextName, contextShortID, contextDescription) pelagicore::LogContext contextName( \
		contextShortID,	\
		contextDescription);

/**
 * Create a new context and define is as default context for the current scope
 */
#define LOG_DECLARE_DEFAULT_CONTEXT(context, contextID, contextDescription) \
	LOG_DECLARE_CONTEXT(context, contextID, contextDescription); \
	LOG_SET_DEFAULT_CONTEXT(context);

/**
 * Import the given context, which should exported by another module
 */
#define LOG_IMPORT_CONTEXT(contextName) extern pelagicore::LogContext contextName;

/**
 * Set the given context as default for the current scope
 */
#define LOG_SET_DEFAULT_CONTEXT(context) static std::function<pelagicore::LogContext& ()> getDefaultContext = []()->pelagicore::LogContext & {return context; };

/**
 * Import the given context and set it as default for the current scope
 */
#define LOG_IMPORT_DEFAULT_CONTEXT(context) LOG_IMPORT_CONTEXT(context); LOG_SET_DEFAULT_CONTEXT(context);

/**
 * Set the given context as default for the current class
 */
#define LOG_SET_CLASS_CONTEXT(context) static inline pelagicore::LogContext & getDefaultContext() { return context; }

/**
 *
 */
#define LOG_DECLARE_DEFAULT_LOCAL_CONTEXT(contextShortID, contextDescription) \
	std::function<pelagicore::LogContext& ()> getDefaultContext = []()->pelagicore::LogContext & { \
		static pelagicore::LogContext __defaultContext( contextShortID, contextDescription); \
		return __defaultContext; \
	}

/**
 *
 */
#define LOG_DECLARE_CLASS_CONTEXT(contextShortID, contextDescription) \
	static pelagicore::LogContext & getDefaultContext() { \
		static pelagicore::LogContext __defaultLogContext( contextShortID, contextDescription); \
	    return __defaultLogContext; \
	}

/**
 * Deprecated
 */
#define LOG_INSTANTIATE_CLASS_CONTEXT(className__)

#endif

}

#include "log-types.h"
