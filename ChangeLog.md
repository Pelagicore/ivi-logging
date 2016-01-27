Change log
==========


1.3.0
-----
- Changed environment variable name from "DISABLE_LOGGING_CONSOLE" to "LOGGING_ENABLE_CONSOLE" for consistency. Set LOGGING_ENABLE_CONSOLE to "0" to disable the logging to the console, if the logging to the console backend is enabled at build time.
- Added possibility to enable source code information output (source file, line number) by setting the "LOGGING_ENABLE_SOURCE_CODE_INFORMATION" environment variable to "1". Note that the logging on the console always show source code information.
- Added possibility to enable thread information output (thread name, thread ID) by setting the "LOGGING_ENABLE_THREAD_INFORMATION" environment variable to "1".

1.1.1
-----
- Setting the environment variable "DISABLE_LOGGING_CONSOLE" to any value disables the output to the console.

1.1.0
-----

- Add possibility to output thread information in logs.
- Support STL's "std::endl", "std::ends", "std::flush"
- Support for Clang compiler

1.0.0 
-----

First version

