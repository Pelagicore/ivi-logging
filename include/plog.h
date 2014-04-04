#pragma once

#include <string>
#include <functional>
#include "string.h"

#include "log-common.h"

namespace pelagicore {

template<size_t I = 0, typename Func, typename ... TupleTypes, typename ... CallArgumentTypes>
typename std::enable_if<I == sizeof ... (TupleTypes)>::type
for_each_in_tuple_(std::tuple<TupleTypes ...>&, Func, CallArgumentTypes& ... args) {
}

template<size_t I = 0, typename Func, typename ... TupleTypes, typename ... CallArgumentTypes>
typename std::enable_if < I<sizeof ... (TupleTypes)>::type
for_each_in_tuple_(std::tuple<TupleTypes ...>& tpl, Func func, CallArgumentTypes& ... args) {
	func(std::get<I>(tpl), args ...);
	for_each_in_tuple_<I + 1>(tpl, func, args ...);
}


inline std::string pointerToString(const void* p) {
	char buffer[32];
	sprintf(buffer, "0x%zX", (size_t) p);
	return buffer;
}

int getThreadID();



#define buildFormatString(format) format

#define log_with_context(context, severity, args ...) \
	for (pelagicore::LogContext* dummy = &context; (dummy != nullptr) && dummy->isEnabled(severity); dummy = nullptr) \
		context.createLog(severity, __FILE__, __LINE__, __PRETTY_FUNCTION__).writeFormatted(args)

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
#define LOG_SET_DEFAULT_CONTEXT(context) static std::function<pelagicore::LogContext& ()> getDefaultContext = \
	[] ()->pelagicore::LogContext & {return context; };

/**
 * Import the given context and set it as default for the current scope
 */
#define LOG_IMPORT_DEFAULT_CONTEXT(context) LOG_IMPORT_CONTEXT(context); LOG_SET_DEFAULT_CONTEXT(context);

/**
 * Set the given context as default for the current class
 */
#define LOG_SET_CLASS_CONTEXT(context) static inline pelagicore::LogContext & getDefaultContext() {return context; }

/**
 *
 */
#define LOG_DECLARE_DEFAULT_LOCAL_CONTEXT(contextShortID, contextDescription) \
	std::function<pelagicore::LogContext& ()> getDefaultContext = [] ()->pelagicore::LogContext & {	\
		static pelagicore::LogContext __defaultContext(contextShortID, contextDescription); \
		return __defaultContext; \
	}

/**
 *
 */
#define LOG_DECLARE_CLASS_CONTEXT(contextShortID, contextDescription) \
	static pelagicore::LogContext & getDefaultContext() { \
		static pelagicore::LogContext __defaultLogContext(contextShortID, contextDescription); \
		return __defaultLogContext; \
	}

/**
 * Deprecated
 */
#define LOG_INSTANTIATE_CLASS_CONTEXT(className__)

#endif

template<typename ... Types>
struct Input {
};

template<typename ... Types>
struct Output {
};

template<class, class> class LogContextT;

template<template<class ...> class ContextTypesClass, class ... ContextTypes,
	 template<class ...> class ContextDataTypesClass, class ... LogDataTypes>
class LogContextT<ContextTypesClass<ContextTypes ...>, ContextDataTypesClass<LogDataTypes ...> > : public LogContextAbstract {

	struct setParentContextFunctor {
		template<typename T, typename ParentType>
		void operator()(T && t, ParentType& parent) {
			t.setParentContext(parent);
		}
	};

	struct isEnabledFunctor {
		isEnabledFunctor(bool& b) : m_isEnabled(b) {
		}
		template<typename T>
		void operator()(T && t, LogLevel level) {
			m_isEnabled |= t.isEnabled(level);
		}
		bool& m_isEnabled;
	};

	struct writeFormattedFunctor {
		template<typename T, typename ... Args>
		void operator()(T && t, const char* format, Args ... args) {
			t.writeFormatted(format, args ...);
		}
	};

	struct registerContextFunctor {
		template<typename T>
		void operator()(T && t) {
			t.registerContext();
		}
	};

	struct initDataFunctor {
		template<typename T>
		void operator()(T && t,
				LogContextT<ContextTypesClass<ContextTypes ...>,
					    ContextDataTypesClass<LogDataTypes ...> >& context,
				LogDataAbstract& log) {
			t.init(context, log);
		}
	};

	struct streamFunctor {
		template<typename T, typename Type>
		void operator()(T && t, const Type& v) {
			t << v;
		}
	};

public:
	class LogData : LogDataAbstract {

		template<size_t I = 0, typename ... CallArgumentTypes>
		typename std::enable_if<I == sizeof ... (ContextTypes)>::type
		for_each_init(std::tuple<LogDataTypes ...>&, LogContextT<ContextTypesClass<ContextTypes ...>,
									 ContextDataTypesClass<LogDataTypes ...> >& context,
			      CallArgumentTypes& ... args) {
		}

		template<size_t I = 0, typename ... CallArgumentTypes>
		typename std::enable_if < I<sizeof ... (ContextTypes)>::type
		for_each_init(std::tuple<LogDataTypes ...>& tpl, LogContextT<ContextTypesClass<ContextTypes ...>,
									     ContextDataTypesClass<LogDataTypes ...> >& context,
			      CallArgumentTypes& ... args) {
			std::get<I>(tpl).init(std::get<I>(context.m_contexts), args ...);
			for_each_init<I + 1>(tpl, context, args ...);
		}

public:
		LogData(LogContextT<ContextTypesClass<ContextTypes ...>,
				    ContextDataTypesClass<LogDataTypes ...> >& context, LogLevel level, const char* fileName,
			int lineNumber,
			const char* prettyFunction) :
			LogDataAbstract(level, fileName, lineNumber, prettyFunction) {
			for_each_init(m_contexts, context, *this);
		}

		~LogData() {
		}

		LogData& writeFormatted() {
			return *this;
		}

		template<typename ... Args>
		LogData& writeFormatted(const char* format, Args ... args) {
			for_each_in_tuple_(m_contexts, writeFormattedFunctor(), format, args ...);
			return *this;
		}

		//template<typename Types ... >
		template<typename Type> void stream(const Type& v) {
			for_each_in_tuple_(m_contexts, streamFunctor(), v);
		}

		template<typename Type> LogData& operator<<(const Type& v) {
			stream(v);
			return *this;
		}

		std::tuple<LogDataTypes ...> m_contexts;
	};

	LogContextT(const char* id, const char* contextDescription) : LogContextAbstract(id, contextDescription) {
		for_each_in_tuple_(m_contexts, setParentContextFunctor(), *this);
	}

	LogData createLog(LogLevel level, const char* fileName, int lineNumber, const char* prettyFunction) {
		return LogData(*this, level, fileName, lineNumber, prettyFunction);
	}

	bool isEnabled(LogLevel level) {
		checkContext();
		bool enabled = false;
		for_each_in_tuple_(m_contexts, isEnabledFunctor(enabled), level);
		return enabled;
	}

	void checkContext() {
		if (!m_bRegistered) {
			registerDefaultAPPIDSIfNeeded();
			for_each_in_tuple_( m_contexts, registerContextFunctor() );
			m_bRegistered = true;
		}

	}

	std::tuple<ContextTypes ...> m_contexts;

};
}
