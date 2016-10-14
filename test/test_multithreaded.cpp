#include <pthread.h>
#include <thread>
#include "test-common.h"

typedef LogContextWithConsolePlusDLTIfAvailable LogContext;

LOG_DECLARE_DEFAULT_CONTEXT(myTestContext, "TEST", "Test context");

int main(int, const char**) {

	new std::thread([&] () {
				pthread_setname_np(pthread_self(), "MyThread2");
				log_warn() << "Test log from thread";
			});

	log_warn() << "Test from main";

	sleep(1);

}
