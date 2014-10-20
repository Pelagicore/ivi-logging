#pragma once

#include <unistd.h>
#include <string>
#include <string.h>
#include <atomic>
#include <array>

namespace logging {

#define LOGGING_WARNING_OUTPUT_PREFIX "Logging: "

enum class LogLevel {
	None, Fatal, Error, Warning, Info, Debug, Verbose, All
};

struct AppLogContext;

extern AppLogContext* s_pAppLogContext;

std::string getProcessName(pid_t pid);

class ThreadInformation {

public:
	ThreadInformation() {
		m_id = sNextID++;
	}

	static bool isMultithreadedApp() {
		return (sNextID != 1);
	}

	int getID() const {
		return m_id;
	}

	const char* getName() const;

private:
	mutable std::string m_name;
	int m_id;
	static std::atomic_int sNextID;

};

ThreadInformation& getThreadInformation();

struct AppLogContext {
	AppLogContext(const char* id, const char* description) : m_id(id), m_description(description) {
		if (s_pAppLogContext != nullptr) {
			fprintf(
				stderr,
				LOGGING_WARNING_OUTPUT_PREFIX
				"An ID has already been defined for your application. Please ensure LOG_DEFINE_APP_IDS macro is called before any log is produced.\n");
		}
		s_pAppLogContext = this;
	}
	std::string m_id;
	std::string m_description;
};

void setDefaultAPPIDSIfNeeded();

/**
 * A logging context
 */
class LogContextCommon {

public:
	LogContextCommon(const char* id, const char* contextDescription) : m_id(id), m_description(contextDescription) {
		if (strlen(id) > 4)
			fprintf(
				stderr,
				LOGGING_WARNING_OUTPUT_PREFIX
				"Log IDs should not be longer than 4 characters to be compatible with the DLT : %s\n",
				id);

	}

	const char* getDescription() const {
		return m_description.c_str();
	}

	const char* getID() const {
		return m_id.c_str();
	}

private:
	const std::string m_id;
	const std::string m_description;

};

class LogDataCommon {

public:
	LogDataCommon(LogLevel level, const char* fileName, int lineNumber, const char* prettyFunction) {
		m_level = level;
		m_longFileName = fileName;
		m_lineNumber = lineNumber;
		m_prettyFunction = prettyFunction;
	}

	LogLevel getLogLevel() const {
		return m_level;
	}

	const char* getFileName() const {
		if (m_fileName == nullptr) {
			size_t shortNamePosition = strlen(m_longFileName);
			while ( (shortNamePosition > 0) && (m_longFileName[shortNamePosition - 1] != '/') )
				shortNamePosition--;
			m_fileName = m_longFileName + shortNamePosition;
		}
		return m_fileName;
	}

	int getLineNumber() {
		return m_lineNumber;
	}

	const char* getPrettyFunction() const {
		return m_prettyFunction;
	}

private:
	const char* m_longFileName;
	mutable const char* m_fileName = nullptr;
	const char* m_prettyFunction;
	int m_lineNumber;
	LogLevel m_level;
};

std::string getStackTrace(unsigned int max_frames = 63);

}
