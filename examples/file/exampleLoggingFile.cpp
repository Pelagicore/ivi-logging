#include "logging-file.h"

// If an application is not multi-instance, we can define its unique identifier
LOG_DEFINE_APP_IDS("MyAp", "This is a small application showing how to use ivi-logging to log to a file");

// Instantiate a log context and define it as default for this module
LOG_DECLARE_DEFAULT_CONTEXT(mainContext, "MAIN", "This is a description of that logging context");

static constexpr const char* FILE_PATH = "/tmp/ivi-logging-test.txt";

int main(int argc, const char** argv) {

    logging::FileLogContext::setFilePath(FILE_PATH);
    log_info() << "This log should be written to a file as well : " << FILE_PATH;
	return 0;

}
