/**

\mainpage Logging

\section Introduction

This package contains utility classes for logging with support for DLT

\section Logging Logging
The logging component enables your application to easily send logs either to the console, to a file, or to the DLT, in a
transparent way, without having to explicitly rely on the DLT API.
A Qt extension automatically adds support for any Qt data type which can be used with the standard Qt logging system (qDebug).

\subsection LogUsage Usage
\include "exampleLogging.cpp"

\section Doxygen Doxygen CMake integration
By adding the following lines to your CMake makefile, a Doxygen documentation will automatically be generated, and
        installed into the standard location for software documentation : "${CMAKE_INSTALL_PREFIX}/share/${PACKAGE}.
        This documentation should eventually end up in a documentation package, if your build toolchain enables that.
\tableofcontents

\section FAQ FAQ
\li TODO

\section ChangeLog Change log
\include ChangeLog.txt

*/
