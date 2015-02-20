#pragma once

#include "ivi-logging-console.h"

namespace logging {

class FileLogData : public StreamLogData {
public:
	~FileLogData() {
		flushLog();
	}

};

class FileLogContext : public StreamLogContextAbstract {
public:
	typedef FileLogData LogDataType;

    FILE* getFile(logging::StreamLogData& data) override {
        return getFileStatic();
    }

    static void openFile(const char* fileName) {
        if (getFileStatic() == nullptr) {
            getFileStatic() = fopen(fileName, "w");
        }
        assert(getFileStatic() != nullptr);
    }

    static FILE*& getFileStatic() {
        static FILE* m_file;
        return m_file;
    }

};

}
