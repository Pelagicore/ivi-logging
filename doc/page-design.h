/**

\page Design

\section Design Design

That section gives you information about the design of the ivi-logging.

\subsection Requirements Requirements

The list of requirements can be found in the main page of that doxygen documentation.
TODO : insert link

\subsection DesignDecision Design decisions

Considering the requirements, the following decisions have been made:
  - The list of logging backends is defined at build time. That allows the compiler to perform optimizations such as inlining most
of the calls the various backends, since all the classes needed to generate and forward the logs are known at build time. As far as
the DLT backend is concerned, that means that the cost of the abstraction is very low. The drawback of that decision is that it is
currently not possible to add a backend at runtime.

\subsection Diagrams

Since the library makes heavy use of C++11 variadic templates, it is pretty hard to provide a generic class diagram.
In IVI systems, the typical use case is that ivi-logging uses two backends : DLT and console (stdout/stderr). That is the use case
which we are going to focus on here.

TODO

*/
