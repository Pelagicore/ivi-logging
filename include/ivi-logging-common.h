#pragma once

#include <unistd.h>
#include <string>
#include <string.h>
#include <atomic>
#include <array>

namespace logging {

inline void UNUSED() {
}

template<typename Type, typename ... Types> void UNUSED(const Type& firstValue, const Types& ... otherValues) {
	(void) firstValue;
	UNUSED(otherValues ...);
}

#define LOGGING_WARNING_OUTPUT_PREFIX "Logging: "

enum class LogLevel {
	None, Fatal, Error, Warning, Info, Debug, Verbose, All, Invalid
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
	LogContextCommon(const std::string& id, const std::string& contextDescription) : m_id(id), m_description(contextDescription) {
		if (id.length() > 4)
			fprintf(
				stderr,
				LOGGING_WARNING_OUTPUT_PREFIX
				"Log IDs should not be longer than 4 characters to be compatible with the DLT : %s\n",
				id.c_str());
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

class LogContextBase {

public:

	bool isSourceCodeLocationInfoEnabled() const {
		return m_enableSourceCodeLocationInfo;
	}

	bool isThreadInfoEnabled() const {
		return m_enableThreadInfo;
	}

	void registerContext();

private:

	static bool m_enableSourceCodeLocationInfo;
	static bool m_enableThreadInfo;
	static bool s_initialized;

};

class LogInfo {

public:
	LogInfo(LogLevel level, const char* fileName, int lineNumber, const char* prettyFunction) {
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

	int getLineNumber() const {
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

class LogData {
};

/// Deprecated : use LogData
__attribute__ ((deprecated)) typedef LogData LogDataCommon;

std::string getStackTrace(unsigned int max_frames = 63);

/// TODO : check if that template method is really inlined by GCC
template<const char* s1, const char* s2, size_t SIZE1 = strlen(s1), size_t SIZE2 = strlen(s2)>
static const char* concatenate() {
    static char s[SIZE1 + SIZE2 + 1];
    for (size_t i = 0; i < SIZE1; i++)
        s[i] = s1[i];
    for (size_t i = 0; i < SIZE2; i++)
        s[SIZE1 + i] = s2[i];
//    memcpy(s, s1, SIZE1);
//    memcpy(s + SIZE1 , s2, SIZE2);
    s[SIZE1 + SIZE2] = 0;
    return s;
}

}
