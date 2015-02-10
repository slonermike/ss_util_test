all:
	g++ main.cpp ss_util/checksum.cpp ss_util/error_reporting.cpp ss_util/math/vector.cpp ss_util/math/sa_math.cpp -o ss_util_test
	
	