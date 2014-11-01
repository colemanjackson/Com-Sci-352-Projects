make: all

all: project1

clean:
		rm -rf project1 testcodemain.o lib-ult.o

tarball: proj1.tar.gz
 

proj1.tar.gz: makefile testcodemain.cc lib-ult.cc
		tar -czf proj1.tar.gz makefile testcodemain.cc lib-ult.cc lib-ult.h

project1: testcodemain.o lib-ult.o
		g++  -ggdb -project1 testcodemain.o lib-ult.o -lpthread

lib-ult.o: lib-ult.h lib-ult.cc
		g++ -ggdb -c lib-ult.cc

testcodemain.o: testcodemain.cc
		g++ -ggdb -c testcodemain.cc



