#include "exampleLogging.h"
#include <unistd.h>

// Each executable needs to define its logging identifiers once
LOG_DEFINE_APP_IDS("MyAp", "This is a small application showing how to use Pelagicore logging");

// Instantiate a log context and define it as default for this module
LOG_DECLARE_DEFAULT_CONTEXT(mainContext, "MAIN", "This is a description of that logging context");

// Instantiate another context which can be used instead of the previous one if specified
LOG_DECLARE_CONTEXT(anotherContext, "CXT2", "Another context");

struct MyClass {

	// Define the log context to be used for that class. This overrides any default context which might have previously be set
	LOG_DECLARE_CLASS_CONTEXT("CCTX", "This is a class-specific context");

	void doSomething() {
		log_debug("We are doing something"); // This log uses the class log context
	}

};

struct MySubClass : MyClass {

	// Define the log context to be used for that class. This overrides any default context which might have previously be set
	LOG_DECLARE_CLASS_CONTEXT("CCT2", "This is a class-specific context for our sub-class");

	void doSomethingElse() {
		log_debug("We are doing something else");
		log_verbose("This is a verbose log");
	}

};

struct MyClassWithImportedContext {

	// Define the log context to be used for that class. This overrides any default context which might have previously be set
	LOG_SET_CLASS_CONTEXT(anotherContext);

	void doSomething() {
		log_debug("We are doing something. Imported context"); // This log uses the class log context
	}

};

void myFunction() {

	// Redefines the context to use for this scope (this function)
	LOG_DECLARE_DEFAULT_LOCAL_CONTEXT("CXT3", "Function-specific logging context");

	log_info("myFunction log 1");
	log_warn("myFunction log 2");

}


int main(int argc, const char** argv) {

	log_error().writeFormatted("Another way to use the printf variant %i", 7345);
	log_warn("Starting app");

	log_debug("This log is using a format string, similar to the printf syntax. This is an int : %i", 345);
	log_error("This one is mixed. Here is a float : ") << 5.7F;
	log_warn() << 5.7;

	myFunction();

	myFunction2();

	MySubClass o;
	o.doSomething();
	o.doSomethingElse();

	MyClassWithImportedContext o2;
	o2.doSomething();

	std::vector<char> charVector;
	for (char i = '0'; i <= '9'; i++)
		charVector.push_back(i);
	log_warn("This is a vector of chars : ") << charVector;

	sleep(1);

}

