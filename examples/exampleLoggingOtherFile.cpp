#include "exampleLogging.h"

LOG_SET_DEFAULT_CONTEXT(anotherContext);

struct MyClass2 {

	// Define the log context to be used for that class. This overrides any default context which might have previously be set
	LOG_SET_CLASS_CONTEXT(anotherContext);

	void doSomething() {
		log_debug("We are doing something in an other file"); // This log uses the class log context
	}

};

void myFunction2() {

	log_info("Using this file's default context");

	MyClass2 o;
	o.doSomething();

}
