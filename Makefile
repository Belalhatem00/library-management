.PHONY: all tests test clean

CXX      = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2
SRC      = src/Book.cpp src/Member.cpp src/Library.cpp

all: library

library: $(SRC) src/main.cpp
	$(CXX) $(CXXFLAGS) $(SRC) src/main.cpp -o library

tests: $(SRC) tests/tests.cpp
	$(CXX) $(CXXFLAGS) $(SRC) tests/tests.cpp -o run_tests

test: tests
	./run_tests

clean:
	rm -f library run_tests
