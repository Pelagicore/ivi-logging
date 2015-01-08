
Dependencies
------------

The following packages need to be installed in order to be able to compile this package
- cmake
- pkg-config
- g++ or clang
- doxygen (for documentation generation)

On Ubuntu or debian, those dependencies can be installed with the following command: 
```
$ sudo apt-get install cmake pkg-config g++ doxygen
```
If you want support for the DLT, that package needs to be installed (http://projects.genivi.org/diagnostic-log-trace/download). Please note that a mistake in DLT's pkg-config file brings a strange dependency to "libz", which you might have to install as well:
```
	$ sudo apt-get install libz-dev
```

Build
-----

Here are instructions on how to build the package:
```
	$ mkdir build
	$ cd build
	$ cmake ../ -DCMAKE_INSTALL_PREFIX=/My/Installation/Location -DENABLE_DOC=1
	$ make install
```


Build artifacts
---------------

When building and installing the package, the following should be installed:
* Development headers
* A library which your software needs to link against in order to use ivi-logging functionality
* A pkg-config file to be used to build your software against ivi-logging
* An example application called "logging-example", which is installed in "/usr/bin" (if your installation prefix is "/usr")


Testing
-------

To test whether the package has been correctly installed, you can start the example application ("/usr/bin/logging-example"), which should simply print a few logs to the console and also to the DLT (only if DLT is installed on your system) and exit.

