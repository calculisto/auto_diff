.PHONY: all clean check

CXXFLAGS+=-g -I../git_submodules/calculisto/array/include
LINK.o=$(LINK.cc)
LDLIBS+=-lfmt

all: check

check: tests
	./tests

tests: tests.o test-dual.o

tests.o: tests.cpp

test-dual.o: test-dual.cpp ../include/calculisto/auto_diff/dual.hpp
	$(CXX) $(CXXFLAGS) -c -o test-dual.o test-dual.cpp

clean: 
	rm -f tests *.o 
