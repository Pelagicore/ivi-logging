#pragma once

#include <unistd.h>
#include <string>

#ifndef DISABLE_CONSOLE_LOGGING
#define ENABLE_CONSOLE_LOGGING 1
#endif

namespace pelagicore {

//static constexpr char* LOGGING_WARNING_OUTPUT_PREFIX = "Logging: ";
#define LOGGING_WARNING_OUTPUT_PREFIX "Logging: "

enum class LogLevel {
	None, Error, Warning, Info, Debug, Verbose
};

struct AppLogContext;

extern AppLogContext* s_pAppLogContext;

std::string getProcessName(pid_t pid);

struct AppLogContext {
	AppLogContext(const char* id, const char* description) : m_id(id), m_description(description) {
		if (s_pAppLogContext != nullptr) {
			fprintf(
				stderr,
				LOGGING_WARNING_OUTPUT_PREFIX
				"Your application should not define its ID twice using LOG_DEFINE_APP_IDS macro\n");
		}
		s_pAppLogContext = this;
	}
	std::string m_id;
	std::string m_description;

#ifdef ENABLE_DLT_LOGGING
	bool dltRegistered = false;
#endif

#ifdef ENABLE_CONSOLE_LOGGING
	bool consoleRegistered = false;
#endif

};

void registerDefaultAPPIDSIfNeeded();

}
