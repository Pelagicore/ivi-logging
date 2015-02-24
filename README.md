
Introduction    {#mainpage}
============

IVI-Logging is a software which facilitates the generation of log data. It provides an easy-to-use C++ API enabling you to send log data to various channels such as to the console, files, or GENIVI's DLT (http://projects.genivi.org/diagnostic-log-trace).
Since application code does not rely directly on any specific logging backend, very little to no modification is needed to change the actual backends used to send the log data.

For more detailed documentation about that software component, please refer to the doxygen documentation, which gets generated when
building the package (see the file "INSTALL.md"). Open the file "doc/doxygen/html/index.html" from the build folder.


Backends
========

The following channels can be used as log output:
* DLT
* Console
* File(s)
* Null (can be useful to completely disable the generation of log data)

Any combination of these backends can be used if several backends are needed (logging to console + DLT for example). 

Fulfilled requirements
======================

The following requirements are fulfilled by that package:
* An API similar to Qt's QDebug and STL's logging APIs should be provided.
* An API similar to libc's printf() should be provided.
* The API used to produce logs should be the same, whatever channel is actually used as output.
* Support the DLT's "context" concept. An application can use several logging contexts. Each context is identified by a 4 characters ID, and a longer textual description. The log level of each context can be adjusted at runtime.
* When logging to DLT only, the C++ API should introduce little to no overhead compared to the DLT's C macro based API.
* It should be possible to build an application with the logging completely disabled, without changing the application code. Once disabled, the application binary code should be as small as if the lines of codes producing the logs were completely removed.
* If no application ID is provided, an ID containing the PID should be automatically assigned.
* When a logging context is disabled, the application should not evaluate the parameters passed to the logging.
* Logging to the console should use color decorations.
* The format of the console or file log entries should be customizable.
* The logs should contain optional source code information (file name, line number).

Default configuration
=====================

The list of backends used to send logs to can be freely set by packages at build time. However, the ivi-logging packages provides a default configuration which can be used "as-is" by packages using the ivi-logging. That provides a way to centralize the configuration so that all packages use the same backends.
Typically, a "production" build would use only the DLT backend, whereas a "development" build would also enable the logging to the console to help debugging in command-line for example. That distinction between the various builds can be centralized in the configuration of ivi-logging instead of being spread over many packages.
Please see the install file for information about how to customize that default configuration : [INSTALL.md](./INSTALL.md)

Usage
=====

In order to start using the logging component from your application, you need to:
* Define a type called "LogContext" and ensure that this type is accessible in the current scope. That type defines the class to be used for the logging. Typical applications would have a file called "logging.h" which reuses the default configuration provided by the ivi-logging package. Example: [logging.h](./examples/basic/logging-basic.h)
* Optionally define the 4 characters unique ID (if only one instance of your application is going to run), which is going to be used especially by the DLT backend as application ID. This is achieved by using the following macro:
```LOG_DEFINE_APP_IDS("MyAp", "Description of my application");```
* Declare at least one logging context by using the following macro:
```LOG_DECLARE_CONTEXT(mainContext, "MAIN", "This is a description of that logging context");```
* Set that context as default context in the current scope. See next section for details.
* Use one of the logging macro to actually generate a log:
  * ```log_debug() << "Here is a debug log";```
  * ```log_error() << "Here is an error log";```
  * ```log_info() << "Here is an info log";```
  * ```log_verbose() << "Here is a verbose log";```

Default context
---------------

In order to use the macros such as log_debug(), a context needs to be set as "default" for the current scope. To set a context as default, several possibilities exist, depending on the situation:
* ```LOG_SET_DEFAULT_CONTEXT(context)```
* ```LOG_DECLARE_CLASS_CONTEXT(ID, Description)```
* ```LOG_DECLARE_DEFAULT_LOCAL_CONTEXT(ID, Description)```

See the example below to understand when each of these macros can be used.

Usage - Examples
================

Here is an example of file producing log data:
[exampleLoggingMain.cpp](./examples/basic/exampleLoggingMain.cpp)


Installation
============

Please see that file : [INSTALL.md](./INSTALL.md)

License
=======

This software is Free Software licensed under the Mozilla Public License 2.0


TODO
====

* It should be possible to enable the output of source code information (file name, line number) dynamically (via the DLT-viewer for example). However, it should also be possible to compile applications without the source code information since that information increases the code size.
* Support for logging to syslog.
* A bridge should be provided, so that logs produced by the Qt runtime can be forwarded to the active logging backends.
