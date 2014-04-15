/**

\mainpage Logging

\section Introduction

This logging component provides a C++ abstract API, which enables your application to send logs either to the console, to a file, or to the DLT, in a
transparent way, without having to explicitly rely on the DLT API.
A Qt extension automatically adds support for any Qt data type which can be used with the standard Qt logging system (qDebug).

\section SupportedBackends Supported backends
The following channels can be used as log output:
        \li DLT
        \li Console
        \li File(s)

\section Req Fulfilled requirements
The following requirements are fulfilled by that package:
        \li An API similar to the QDebug API from Qt should be provided.
        \li A formatting API taking a printf-like formatting should be provided.
        \li The API used to produce a log entry should be the same, whatever channel is actually used as output.
        \li Support the DLT's "context" concept. An application can use several logging contexts. Each context is identified by a 4 characters ID, and a longer textual description.
        \li When logging to DLT only, the C++ API should introduce little to no overhead compared to the DLT's C macro based API.
        \li When a logging context is disabled, the application should not evaluate the parameters passed to the logging.
        \li Logging to the console should use color decorations.
        \li The format of the console or file log entries should be customizable.
        \li A bridge should be provided, so that logs produced by the Qt runtime can be forwarded to the active logging backends.

\section TODO
        \li Logging to syslog.

\section LogUsage Usage
In order to start using the logging component from your application, you need to:
        \li Include the "plog.h" header  // TODO : change header name
        \li Optionally define the 4 characters ID, which is going to be used especially by the DLT backend as application ID. This is achieved by using the following macro:\n
                LOG_DEFINE_APP_IDS("MyAp", "Description of my application");
        \li Declare at least one logging context by using the following macro:\n
                LOG_DECLARE_CONTEXT(mainContext, "MAIN", "This is a description of that logging context");
        \li Set that context as default context in the current scope. See next section for details.
        \li Use one of the logging macro to actually generate a log:
                 - log_debug() << "Here is a debug log";
                 - log_error() << "Here is an error log";
                 - log_info() << "Here is an info log";
                 - log_verbose() << "Here is a verbose log";

\subsection Scope Default context
        In order to use the macros such as log_debug(), a context needs to be set as "default" for the current scope. To set a context as default, several possibilities exist, depending on the situation:
                - LOG_SET_DEFAULT_CONTEXT(context)
				-
				-
                - LOG_DECLARE_CLASS_CONTEXT(ID, Description)

\section Example Example
\include "exampleLoggingMain.cpp"

\tableofcontents

\section FAQ FAQ
\li TODO

\section ChangeLog Change log
\include ChangeLog.txt

*/
