
Dependencies
------------

The following packages need to be installed in order to be able to compile this package
- cmake
- pkg-config
- g++
- doxygen (for documentation generation)

On Ubuntu or debian, those dependencies can be installed with the following command: 
	$ sudo apt-get install cmake pkg-config g++ doxygen

If you want support for the DLT, that package needs to be installed : http://projects.genivi.org/diagnostic-log-trace/download
Note that a mistake in DLT's pkg-config file brings a dependency to "libz", which you might have to install as well:
	$ sudo apt-get install libz-dev

Build
-----

Here are instructions on how to build the package:
	$ mkdir build
	$ cd build
	$ cmake ../ -DCMAKE_INSTALL_PREFIX=/My/Installation/Location -DENABLE_DOC=1
	$ make install

