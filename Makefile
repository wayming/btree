all: build run

build:
	g++ -std=c++2a -D__cpp_lib_shift -c btree.cc

run:
	cd gtest; make all

clean:
	rm -rf *.o