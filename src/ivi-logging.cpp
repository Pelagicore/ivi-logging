#include "stdint.h"
#include "stdio.h"
#include "ivi-logging-console.h"
#include "ivi-logging-common.h"
#include <string>
#include <dirent.h>
#include <sys/ioctl.h>
#include <atomic>

namespace logging {

AppLogContext* s_pAppLogContext = nullptr;

LogLevel ConsoleLogContext::s_defaultLogLevel = LogLevel::All;

std::mutex StreamLogContextAbstract::m_outputMutex;

std::atomic<uint32_t> sNextThreadId = ATOMIC_VAR_INIT(0);

__thread uint32_t __threadId=0;

uint32_t getThreadID() {
    if (!__threadId){
        __threadId = ++sNextThreadId;
    }
	return __threadId;
}

__thread const char *LogDataCommon::m_cachedFileName = nullptr;
__thread const char *LogDataCommon::m_cachedLongFileName = nullptr;

void setDefaultAPPIDSIfNeeded() {
	if (s_pAppLogContext == nullptr) {
		//		fprintf(
		//			stderr,
		//			LOGGING_WARNING_OUTPUT_PREFIX
		//			"Your application should define its ID using the LOG_DEFINE_APP_IDS macro\n");
		pid_t pid = getpid();
		char pidAsHex[5];
		snprintf(pidAsHex, sizeof(pidAsHex), "%X", pid);
		//		char pidAsDecimal[6];
		//		snprintf(pidAsDecimal, sizeof(pidAsDecimal), "%i", pid);
		//		std::string processName = "PID:";
		//		processName += pidAsDecimal;
		//		processName += " / ";
		//		std::string processName = getProcessName(pid);
		s_pAppLogContext = new AppLogContext( pidAsHex, getProcessName(pid).c_str() );
	}
}

std::string byteArrayToString(const void* buffer, size_t length) {
	static char hexcode[] = "0123456789ABCDEF";
	static char textBuffer[1024];

	size_t dest = 0;

	if (length + 1 > sizeof(textBuffer) / 3)
		length = sizeof(textBuffer) / 3;

	const unsigned char* bufferAsChar = static_cast<const unsigned char*>(buffer);

	for (size_t byteIndex = 0; byteIndex < length; byteIndex++) {
		textBuffer[dest++] = hexcode[bufferAsChar[byteIndex] >> 4];
		textBuffer[dest++] = hexcode[bufferAsChar[byteIndex] & 0xF];
		textBuffer[dest++] = ' ';
	}

	textBuffer[dest] = 0;

	return textBuffer;
}

unsigned int StreamLogContextAbstract::getConsoleWidth() {
	struct::winsize ws;
	if (::ioctl(0, TIOCGWINSZ, &ws) == 0) {
		return ws.ws_col;
	} else
		return 80;
}

std::string getProcessName(pid_t pid) {

	DIR* dir;
	FILE* file;

	if (pid < 0)
		return NULL;

	dir = opendir("/proc");
	if (!dir) {
		char b[1024];
		snprintf(b, sizeof(b), "Unknown process with PID %i", pid);
		return b;
	}

	char path[64];
	snprintf(path, sizeof(path), "/proc/%i/cmdline", pid);
	file = fopen(path, "r");

	if (file == NULL) {
		closedir(dir);
		return "Unknown process";
	}

	char processName[256];
	fread(processName, 1, sizeof(processName) - 1, file);
	fclose(file);
	closedir(dir);

	return processName;
}

}
