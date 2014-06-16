
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++11")

#add_executable( logging-test
#	test.cpp
#	test2.cpp
#)

#TARGET_LINK_LIBRARIES( logging-test
#   ${LIB_NAME}
#)

add_executable( logging-test-multithreaded
	test_multithreaded.cpp
)

TARGET_LINK_LIBRARIES( logging-test-multithreaded
   ${LIB_NAME}
)
