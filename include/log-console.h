#pragma once

#include "log-common.h"
#include "stdio.h"
#include "ByteArray.h"
#include <mutex>

#ifndef LOG_CONSOLE_PREFIX_FORMAT_STRING
	#define LOG_CONSOLE_PREFIX_FORMAT_STRING "[%s] %.4s | "
#endif
#ifndef LOG_CONSOLE_SUFFIX_FORMAT_STRING
	#define LOG_CONSOLE_SUFFIX_FORMAT_STRING "  %s / %s - %d"
#endif

#define LOG_FILE_SET_OUTPUT(file)

namespace pelagicore {

// If no severity is already specified, set it to debug
#ifndef LOG_CONSOLE_SEVERITY
#define LOG_CONSOLE_SEVERITY LogLevel::Debug
#endif

class ConsoleLogOutput {

public:
	ConsoleLogOutput(FILE* file) {
		m_file = file;
	}

	ConsoleLogOutput(const char* path) {
		m_file = fopen(path, "w");
		m_path = path;
	}

	FILE* getFile() {
		return m_file;
	}

	static void setInstance(ConsoleLogOutput& logManager) {
		s_instance = &logManager;
	}

	static ConsoleLogOutput& getInstance() {
		return *s_instance;
	}

	const char* getPath() {
		return m_path.c_str();
	}

private:
	FILE* m_file;
	std::string m_path;
	static ConsoleLogOutput* s_instance;

};

class ConsoleLogContext {
public:
	ConsoleLogContext(const char* id, const char* contextDescription) {
		m_id = id;
		m_description = contextDescription;
	}

	const char* getDescription() {
		return m_description;
	}

	const char* getShortID() {
		return m_id;
	}

	FILE* getFile() {
		return ConsoleLogOutput::getInstance().getFile();
	}

	bool isEnabled(LogLevel level) {
		return (level <= LOG_CONSOLE_SEVERITY);
	}

	void write(const void* s) {
		std::lock_guard<std::mutex> lock(m_outputMutex);
		fprintf(getFile(), "%s\n", (const char*)s);
	}

	const char* m_id;
	const char* m_description;
	int m_fd = -1;

	std::mutex m_outputMutex;
};

class ConsoleLogData {

	enum class Command {
		RESET = 0, BRIGHT = 1, DIM = 2, UNDERLINE = 3, BLINK = 4, REVERSE = 7, HIDDEN = 8
	};

	enum class Color {
		BLACK = 0, RED = 1, GREEN = 2, YELLOW = 3, BLUE = 4, MAGENTA = 5, CYAN = 6, WHITE = 7
	};

public:
	ConsoleLogData(ConsoleLogContext& aContext, LogLevel logLevel, const char* fileName, int lineNumber,
		       const char* prettyFunction) :
		m_context(aContext), m_logLevel(logLevel) {

		m_fileName = fileName;
		m_lineNumber = lineNumber;
		m_prettyFunction = prettyFunction;

		if ( isEnabled() ) {
			changeCurrentColor(Command::RESET, getColor(logLevel), Color::BLACK);
			writeFormatted( LOG_CONSOLE_PREFIX_FORMAT_STRING, getLogLevelString(logLevel), aContext.getShortID() );
		}
	}

	void changeCurrentColor(Command attr, Color fg, Color bg) {
		/* Command is the control command to the terminal */
		writeFormatted("%c[%d;%d;%dm", 0x1B, attr, static_cast<int>(fg) + 30, static_cast<int>(bg) + 40);
	}

	static const char* getLogLevelString(LogLevel logLevel) {
		const char* v = "";
		switch (logLevel) {
		case LogLevel::Debug : v = "Debug  "; break;
		case LogLevel::Info : v = "Info   "; break;
		case LogLevel::Warning : v = "Warning"; break;
		case LogLevel::Error : v = "Error  "; break;
		case LogLevel::Verbose : v = "Verbose"; break;
		default : v = "unknown"; break;
		}
		return v;
	}

	static Color getColor(LogLevel logLevel) {
		Color c = Color::WHITE;
		switch (logLevel) {
		case LogLevel::Warning : c = Color::RED; break;
		case LogLevel::Error : c = Color::RED; break;
		case LogLevel::Verbose : c = Color::GREEN; break;
		default :         c = Color::WHITE;
		}
		return c;
	}

	~ConsoleLogData() {
		// flush
		if ( isEnabled() ) {
			writeFormatted(LOG_CONSOLE_SUFFIX_FORMAT_STRING, m_fileName, m_prettyFunction, m_lineNumber);
			// add terminal null character
			m_content.resize(m_content.size() + 1);
			m_content[m_content.size() - 1] = 0;
			changeCurrentColor(Command::RESET, getColor(LogLevel::None), Color::BLACK);
			m_context.write( m_content.getData() );
			//			fprintf(getFile(), "%s\n", m_content.getData());
		}
	}

	bool isEnabled() {
		return (m_logLevel <= LOG_CONSOLE_SEVERITY);
	}

	void write() {
	}

	template<typename Arg1, typename ... Args>
	void write(Arg1 firstArg, Args ... remainingArguments) {
		if ( isEnabled() ) {
			operator<<(firstArg);
			write(remainingArguments ...);
		}
	}

	template<typename ... Args>
	void writeFormatted(const char* format, Args ... args) {

	#pragma GCC diagnostic push
		// Make sure GCC does not complain about not being able to check the format string since it is no literal string
	#pragma GCC diagnostic ignored "-Wformat-security"
		if ( isEnabled() ) {
			int size = snprintf(NULL, 0, format, args ...) + 1; // +1 since the snprintf returns the number of characters excluding the null termination
			size_t startOfStringIndex = m_content.size();
			m_content.resize(m_content.size() + size);
			char* p = (char*) (m_content.getData() + startOfStringIndex);
			snprintf(p, size, format, args ...);

			// remove terminal null character
			m_content.resize(m_content.size() - 1);
		}
	#pragma GCC diagnostic pop

	}

private:
	ConsoleLogContext& m_context;
	LogLevel m_logLevel;
	ByteArray m_content;

	const char* m_fileName;
	int m_lineNumber;
	const char* m_prettyFunction;
};

inline ConsoleLogData& operator<<(ConsoleLogData& data, bool v) {
	if ( data.isEnabled() )
		data.writeFormatted("%s", v ? "true" : "false");
	return data;
}

inline ConsoleLogData& operator<<(ConsoleLogData& data, char v) {
	if ( data.isEnabled() )
		data.writeFormatted("%d", v);
	return data;
}

inline ConsoleLogData& operator<<(ConsoleLogData& data, unsigned char v) {
	if ( data.isEnabled() )
		data.writeFormatted("%u", v);
	return data;
}

inline ConsoleLogData& operator<<(ConsoleLogData& data, signed short v) {
	if ( data.isEnabled() )
		data.writeFormatted("%d", v);
	return data;
}

inline ConsoleLogData& operator<<(ConsoleLogData& data, unsigned short v) {
	if ( data.isEnabled() )
		data.writeFormatted("%u", v);
	return data;
}

inline ConsoleLogData& operator<<(ConsoleLogData& data, signed int v) {
	if ( data.isEnabled() )
		data.writeFormatted("%d", v);
	return data;
}

inline ConsoleLogData& operator<<(ConsoleLogData& data, unsigned int v) {
	if ( data.isEnabled() )
		data.writeFormatted("%u", v);
	return data;
}

inline ConsoleLogData& operator<<(ConsoleLogData& data, signed long v) {
	if ( data.isEnabled() )
		data.writeFormatted("%ld", v);
	return data;
}

inline ConsoleLogData& operator<<(ConsoleLogData& data, unsigned long v) {
	if ( data.isEnabled() )
		data.writeFormatted("%lu", v);
	return data;
}

inline ConsoleLogData& operator<<(ConsoleLogData& data, const char* v) {
	if ( data.isEnabled() )
		data.writeFormatted("%s", v ? v : "null");
	return data;
}

inline ConsoleLogData& operator<<(ConsoleLogData& data, float v) {
	if ( data.isEnabled() )
		data.writeFormatted("%f", v);
	return data;
}

inline ConsoleLogData& operator<<(ConsoleLogData& data, double v) {
	if ( data.isEnabled() )
		data.writeFormatted("%f", v);
	return data;
}

}
