CXX        = g++
CXXFLAGS   = -Wall -O2 -pedantic -pedantic-errors -Wno-unused-variable -Wno-unused-function -std=c++11
EXECUTABLE = bin2cpp
EXECUTABLE_TEST = test
EXECUTABLE_HW = helloworld

all: bin2cpp make_cpp test

main.o: src/main.cpp
	$(CXX) $(CXXFLAGS) -c -o gen/main.o src/main.cpp

b2cpp.o: src/b2cpp.cpp
	$(CXX) $(CXXFLAGS) -c -o gen/b2cpp.o src/b2cpp.cpp

commandlineparser.o: src/commandlineparser.cpp
	$(CXX) $(CXXFLAGS) -c -o gen/commandlineparser.o src/commandlineparser.cpp

bin2cpp: b2cpp.o commandlineparser.o main.o
	$(CXX) -o bin/$(EXECUTABLE) gen/main.o gen/b2cpp.o gen/commandlineparser.o

make_cpp: bin2cpp helloworld
	bin/bin2cpp --out-dir src --out-file Binaries_Data_Header.h images/gomer.png bin/helloworld

test: test.o
	$(CXX) -o bin/$(EXECUTABLE_TEST) gen/test.o

test.o: src/test.cpp
	$(CXX) $(CXXFLAGS) -c -o gen/test.o src/test.cpp

helloworld: helloworld.o
	$(CXX) -o bin/$(EXECUTABLE_HW) gen/helloworld.o

helloworld.o: src/helloworld.cpp
	$(CXX) $(CXXFLAGS) -c -o gen/helloworld.o src/helloworld.cpp

clean:
	rm -f gen/*
	rm -f bin/*
	rm src/Binaries_Data_Header.h
