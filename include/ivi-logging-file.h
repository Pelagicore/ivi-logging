#pragma once

#include "ivi-logging-console.h"

namespace logging {

class FileLogData : public StreamLogData {
};

class FileLogContext : public StreamLogContextAbstract {
public:
	typedef FileLogData LogDataType;
};

}
