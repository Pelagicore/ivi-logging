#include "stdint.h"
#include "stdio.h"
#include "log-console.h"
#include <string>
#include <dirent.h>

namespace pelagicore {

AppLogContext* s_pAppLogContext = nullptr;

struct ThreadInformation {

	ThreadInformation() {
		id = sNextID++;
	}

	int id = 0;

	static int sNextID;
};
int ThreadInformation::sNextID = 0;

//thread_local
ThreadInformation __threadID;

int getThreadID() {
	return __threadID.id;
}

void setDefaultAPPIDSIfNeeded() {
	if (s_pAppLogContext == nullptr) {
		fprintf(
			stderr,
			LOGGING_WARNING_OUTPUT_PREFIX
			"Your application should define its ID using the LOG_DEFINE_APP_IDS macro\n");
		char pid[5];
		snprintf( pid, sizeof(pid), "%i", getpid() );
		std::string processName = "PID:";
		processName += pid;
		processName += " / ";
		processName += getProcessName( getpid() );
		s_pAppLogContext = new AppLogContext( pid, processName.c_str() );
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
