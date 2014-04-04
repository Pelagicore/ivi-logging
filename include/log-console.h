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

	const char* getPath() {
		return m_path.c_str();
	}

private:
	FILE* m_file;
	std::string m_path;

};

class ConsoleLogData;

class ConsoleLogContextAbstract {
public:
	typedef ConsoleLogData LogDataType;

	ConsoleLogContextAbstract() {
	}

	void setParentContext(LogContextAbstract& context) {
		m_context = &context;
	}

	const char* getShortID() {
		return m_context->m_id;
	}

	void registerContext() {
	}

	virtual FILE* getFile() = 0;

	bool isEnabled(LogLevel level) {
		return (level <= m_level);
	}

	void setLogLevel(LogLevel level) {
		m_level = level;
	}

	void write(const void* s) {
		std::lock_guard<std::mutex> lock(m_outputMutex);
		auto file = getFile();
		if (file) {
			fprintf(file, "%s\n", (const char*)s);
			fflush(file);
		}
	}

	bool colorsEnabled() {
		return m_colorsEnabled;
	}

	std::mutex m_outputMutex;
	LogContextAbstract* m_context = nullptr;
	LogLevel m_level = LOG_CONSOLE_SEVERITY;
	bool m_colorsEnabled = true;
};


class ConsoleLogContext : public ConsoleLogContextAbstract {

public:

	FILE* getFile() override {
		return m_file;
	}

private:
	FILE* m_file = stdout;

};


class ConsoleLogData {

	enum class Command {
		RESET = 0, BRIGHT = 1, DIM = 2, UNDERLINE = 3, BLINK = 4, REVERSE = 7, HIDDEN = 8
	};

	enum class Color {
		BLACK = 0, RED = 1, GREEN = 2, YELLOW = 3, BLUE = 4, MAGENTA = 5, CYAN = 6, WHITE = 7
	};

public:
	typedef ConsoleLogContextAbstract ContextType;

	ConsoleLogData() {
	}

	void init(ConsoleLogContextAbstract& aContext, LogDataAbstract& data) {
		m_context = &aContext;
		m_data = &data;

		if ( isEnabled() ) {
			changeCurrentColor(Command::RESET, getColor(m_data->m_level), Color::BLACK);
			writeFormatted( LOG_CONSOLE_PREFIX_FORMAT_STRING, getLogLevelString(m_data->m_level), aContext.getShortID() );
		}
	}

	void changeCurrentColor(Command attr, Color fg, Color bg) {
		/* Command is the control command to the terminal */
		if (m_context->colorsEnabled())
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
			writeFormatted(LOG_CONSOLE_SUFFIX_FORMAT_STRING, m_data->m_fileName, m_data->m_prettyFunction,
				       m_data->m_lineNumber);
			// add terminal null character
			m_content.resize(m_content.size() + 1);
			m_content[m_content.size() - 1] = 0;
			changeCurrentColor(Command::RESET, getColor(LogLevel::None), Color::BLACK);
			m_context->write( m_content.getData() );
			//			fprintf(getFile(), "%s\n", m_content.getData());
		}
	}

	bool isEnabled() {
		return (m_data->m_level <= LOG_CONSOLE_SEVERITY);
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
	ContextType* m_context;
	ByteArray m_content;
	LogDataAbstract* m_data = nullptr;
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
