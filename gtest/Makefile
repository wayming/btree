all: build run

build:
	g++ -std=c++2a -I${GTEST_BASE}/include -I${BTREE_BASE} -L${GTEST_BASE}/lib64 btree_test.cc -lgtest -lpthread -o btree_test

run:
	./btree_test

clean:
	rm -rf *.o btree_test