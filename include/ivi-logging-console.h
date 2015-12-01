#pragma once

#include "ivi-logging-common.h"
#include "stdio.h"
#include <mutex>
#include "ivi-logging-utils.h"

namespace logging {

class StreamLogData;
class ConsoleLogData;

static constexpr const char ANSI_COLOR_OFF[] = "\x1b[0m";
static constexpr const char ANSI_COLOR_BLACK[] = "\x1b[30m";
static constexpr const char ANSI_COLOR_RED[] = "\x1b[31m";
static constexpr const char ANSI_COLOR_GREEN[] = "\x1b[32m";
static constexpr const char ANSI_COLOR_YELLOW[] = "\x1b[33m";
static constexpr const char ANSI_COLOR_BLUE[] = "\x1b[34m";
static constexpr const char ANSI_COLOR_MAGENTA[] = "\x1b[35m";
static constexpr const char ANSI_COLOR_CYAN[] = "\x1b[36m";
static constexpr const char ANSI_COLOR_GRAY[] = "\x1b[37m";
static constexpr const char ANSI_COLOR_BRIGHT[] = "\x1b[1m";
static constexpr const char ANSI_COLOR_DIM[] = "\x1b[2m";
static constexpr const char ANSI_BLINK[] = "\x1b[5m";
static constexpr const char ANSI_RESET_BRIGHT[] = "\x1b[0m";

class StreamLogContextAbstract {
public:
	StreamLogContextAbstract() {
	}

	virtual ~StreamLogContextAbstract() {
	}

	void setParentContext(LogContextCommon& context) {
		m_context = &context;
	}

	const char* getShortID() {
		return m_context->getID();
	}

	void registerContext() {
	}

	virtual FILE* getFile(StreamLogData& data) = 0;

	void setLogLevel(LogLevel level) {
		m_level = level;
	}

	LogLevel getLogLevel() const {
		return m_level;
	}

	virtual bool isEnabled(LogLevel level) const {
		return ( level <= getLogLevel() );
	}

	void write(const char* s, StreamLogData& data) {
		std::lock_guard<std::mutex> lock(m_outputMutex);
		auto file = getFile(data);
		if (file) {
			fprintf(file, "%s", s);
			fflush(file);
		}
	}

	static unsigned int getConsoleWidth();

private:
	static std::mutex m_outputMutex;
	LogContextCommon* m_context = nullptr;
	LogLevel m_level = LogLevel::Debug;
};

/**
 * Context for logging to the console
 */
class ConsoleLogContext : public StreamLogContextAbstract {

public:
	typedef ConsoleLogData LogDataType;

	static const int DEFAULT_WIDTH = 150;

	static constexpr const char* DISABLE_LOGGING_ENV_VAR_NAME = "DISABLE_LOGGING_CONSOLE";

	ConsoleLogContext() {
		m_colorSupport = (getConsoleWidth() != 0);
		if(!s_envVarCheckDone) {
			if (getenv(DISABLE_LOGGING_ENV_VAR_NAME) != nullptr) {
				s_defaultLogLevel = LogLevel::None;
			}
			s_envVarCheckDone = true;
		}
	}

	bool isEnabled(LogLevel level) const override {
		return ( StreamLogContextAbstract::isEnabled(level) && (level <= s_defaultLogLevel) );
	}

	/**
	 * Sets the global log level for the logging to the console
	 * By setting that log level to "None", you ensure that no log will be sent to the console
	 * By setting that log level to "All" (default value), you disable the effect of the global filtering, which means
	 * that only the log levels of the individual contexts will be taken into account.
	 */
	static void setGlobalLogLevel(LogLevel level) {
		s_defaultLogLevel = level;
	}

	FILE* getFile(StreamLogData& data) override;

	bool isColorsEnabled() {
		return m_colorSupport;
	}

private:
	static LogLevel s_defaultLogLevel;
	static bool s_envVarCheckDone;
	bool m_colorSupport;
};


class StreamLogData : public LogData {

public:
	static constexpr const char* DEFAULT_PREFIX = "%4.4s [%s] ";

	static constexpr const char* DEFAULT_SUFFIX_WITH_FILE_LOCATION = " [ %.2i | %s / %s - %d ]\n";
	static constexpr const char* DEFAULT_SUFFIX_WITH_SHORT_FILE_LOCATION_WITHOUT_FUNCTION = " [ %s%.0s:%d | %.2i%.0s ]\n";
	static constexpr const char* DEFAULT_SUFFIX_WITH_SHORT_FILE_LOCATION_WITHOUT_FUNCTION_WITH_THREAD_NAME = " [ %s%.0s - %d | %.2i-%.16s ]\n";
	static constexpr const char* DEFAULT_SUFFIX_WITHOUT_FILE_LOCATION = "\n";

	typedef StreamLogContextAbstract ContextType;

	StreamLogData() {
	}

	virtual ~StreamLogData() {
	}

	void flushLog() {
		if ( isEnabled() ) {
			writeSuffix();

			// add terminal null character
			m_content.resize(m_content.size() + 1);
			m_content[m_content.size() - 1] = 0;
			m_context->write( (char*)m_content.getData(), *this );
		}
	}

	void setPrefixFormat(const char* format) {
		m_prefixFormat = format;
	}

	void setSuffixFormat(const char* format) {
		m_suffixFormat = format;
	}

	void init(StreamLogContextAbstract& aContext, LogInfo& data) {
		m_context = &aContext;
		m_data = &data;

		if ( isEnabled() )
			writePrefix();

	}

	virtual void writePrefix() {
		writeFormatted( m_prefixFormat, getContext()->getShortID(), getLogLevelString( m_data->getLogLevel() ) );
	}

	virtual void writeSuffix() {
		writeFormatted( getSuffix().getData() );
	}

	virtual ByteArray getSuffix() {
		ByteArray array;
		writeFormatted( array, m_suffixFormat, m_data->getFileName(), m_data->getPrettyFunction(),
				m_data->getLineNumber(), getThreadInformation().getID(), getThreadInformation().getName() );
		return array;
	}

	ContextType* getContext() {
		return m_context;
	}

	static const char* getLogLevelString(LogLevel logLevel) {
		const char* v = "";
		switch (logLevel) {
		case LogLevel::Debug : v = " Debug "; break;
		case LogLevel::Info : v = " Info  "; break;
		case LogLevel::Warning : v = "Warning"; break;
		case LogLevel::Error : v = " Error "; break;
		case LogLevel::Fatal : v = " Fatal "; break;
		case LogLevel::Verbose : v = "Verbose"; break;
		default : v = "unknown"; break;
		}
		return v;
	}

	bool isEnabled() {
		return ( m_context->isEnabled( m_data->getLogLevel() ) );
	}

	LogInfo& getData() {
		return *m_data;
	}

	template<typename ... Args>
	void writeFormatted(const char* format, Args ... args) {
		if ( isEnabled() ) {
			writeFormatted(m_content, format, args ...);
		}
	}

	template<typename ... Args>
	void writeFormatted(ByteArray& byteArray, const char* format, Args ... args) const {

#pragma GCC diagnostic push
		// Make sure GCC does not complain about not being able to check the format string since it is no literal string
#pragma GCC diagnostic ignored "-Wformat-security"
		int size = snprintf(NULL, 0, format, args ...) + 1; // +1 since the snprintf returns the number of characters excluding the null termination
		size_t startOfStringIndex = byteArray.size();
		byteArray.resize(byteArray.size() + size);
		char* p = byteArray.getData() + startOfStringIndex;
		snprintf(p, size, format, args ...);

		// remove terminal null character
		byteArray.resize(byteArray.size() - 1);
#pragma GCC diagnostic pop
	}

protected:
	ContextType* m_context = nullptr;
	ByteArray m_content;
	LogInfo* m_data = nullptr;

	const char* m_prefixFormat = DEFAULT_PREFIX;
	//	const char* m_suffixFormat = DEFAULT_SUFFIX_WITHOUT_FILE_LOCATION;
//	const char* m_suffixFormat = DEFAULT_SUFFIX_WITH_SHORT_FILE_LOCATION_WITHOUT_FUNCTION_WITH_THREAD_NAME;
	const char* m_suffixFormat = DEFAULT_SUFFIX_WITH_SHORT_FILE_LOCATION_WITHOUT_FUNCTION;

};

inline FILE* ConsoleLogContext::getFile(StreamLogData& data) {
	if (data.getData().getLogLevel() == LogLevel::Error)
		return stderr;
	else
		return stdout;
}


inline StreamLogData& operator<<(StreamLogData& data, bool v) {
	if ( data.isEnabled() )
		data.writeFormatted("%s", v ? "true" : "false");
	return data;
}

inline StreamLogData& operator<<(StreamLogData& data, char v) {
	if ( data.isEnabled() )
		data.writeFormatted("%d", v);
	return data;
}

inline StreamLogData& operator<<(StreamLogData& data, unsigned char v) {
	if ( data.isEnabled() )
		data.writeFormatted("%u", v);
	return data;
}

inline StreamLogData& operator<<(StreamLogData& data, signed short v) {
	if ( data.isEnabled() )
		data.writeFormatted("%d", v);
	return data;
}

inline StreamLogData& operator<<(StreamLogData& data, unsigned short v) {
	if ( data.isEnabled() )
		data.writeFormatted("%u", v);
	return data;
}

inline StreamLogData& operator<<(StreamLogData& data, signed int v) {
	if ( data.isEnabled() )
		data.writeFormatted("%d", v);
	return data;
}

inline StreamLogData& operator<<(StreamLogData& data, unsigned int v) {
	if ( data.isEnabled() )
		data.writeFormatted("%u", v);
	return data;
}

inline StreamLogData& operator<<(StreamLogData& data, signed long v) {
	if ( data.isEnabled() )
		data.writeFormatted("%ld", v);
	return data;
}

inline StreamLogData& operator<<(StreamLogData& data, unsigned long v) {
	if ( data.isEnabled() )
		data.writeFormatted("%lu", v);
	return data;
}

inline StreamLogData& operator<<(StreamLogData& data, const char* v) {
	if ( data.isEnabled() )
		data.writeFormatted("%s", v ? v : "null");
	return data;
}

template<size_t N>
inline StreamLogData& operator<<(StreamLogData& data, const char (&v)[N]) {
   data << (const char*) v;
   return data;
}

inline StreamLogData& operator<<(StreamLogData& data, float v) {
	if ( data.isEnabled() )
		data.writeFormatted("%f", v);
	return data;
}

inline StreamLogData& operator<<(StreamLogData& data, const std::string& s) {
	if ( data.isEnabled() )
		data.writeFormatted("%s", s.c_str());
	return data;
}

inline StreamLogData& operator<<(StreamLogData& data, double v) {
	if ( data.isEnabled() )
		data.writeFormatted("%f", v);
	return data;
}

class ConsoleLogData : public StreamLogData {

public:
	typedef ConsoleLogContext ContextType;

	virtual ~ConsoleLogData() {
		flushLog();
	}

	void init(ContextType& aContext, LogInfo& data) {
		m_context = &aContext;
		StreamLogData::init(aContext, data);
	}

	enum class Command {
		RESET = 0, BRIGHT = 1, DIM = 2, UNDERLINE = 3, BLINK = 4, REVERSE = 7, HIDDEN = 8
	};

	enum class Color {
		BLACK = 0, RED = 1, GREEN = 2, YELLOW = 3, BLUE = 4, MAGENTA = 5, CYAN = 6, WHITE = 7
	};

	virtual void writePrefix() override {
	    writeHeaderColor();
		StreamLogData::writePrefix();
        resetColor();
	}

	virtual void writeSuffix() override {

	    writeFooterColor();

		ByteArray suffixArray = getSuffix();

		int width = m_context->getConsoleWidth();

		// If no width is available, use default width
		if (width == 0)
			width = ContextType::DEFAULT_WIDTH;

		width -= m_content.size() + suffixArray.size() - m_invisibleCharacterCount;

		// If the output line is longer that the console width, we print our suffix on the next line
		if (width < 0) {
			writeFormatted("\n");
			width = m_context->getConsoleWidth() - static_cast<int>(suffixArray.size());
		}

		for(int i = 0; i < width; i++) {
			writeFormatted(" ");
		}

		writeFormatted( "%s", suffixArray.getData() );

        resetColor();

	}

    void writeHeaderColor() {
        if ( !m_context->isColorsEnabled() )
            return;

        const char* s = ANSI_COLOR_OFF;
        switch (m_data->getLogLevel()) {
        case LogLevel::Warning : s = ANSI_COLOR_MAGENTA ; break;
        case LogLevel::Error :
        case LogLevel::Fatal: s = concatenate<ANSI_COLOR_RED, ANSI_COLOR_BRIGHT>(); break;
        case LogLevel::Verbose : s = ANSI_COLOR_GREEN; break;
        default: s = ANSI_COLOR_OFF; break;
        }
        *this << s;
        m_invisibleCharacterCount += strlen(s);
    }

    void writeFooterColor() {
        writeHeaderColor();
    }

    void resetColor() {
        if ( !m_context->isColorsEnabled() )
            return;

        auto s = concatenate<ANSI_COLOR_OFF, ANSI_RESET_BRIGHT>();
        *this << s;
        m_invisibleCharacterCount += strlen(s);
    }

	int m_invisibleCharacterCount = 0;
	ContextType* m_context;

};

}
