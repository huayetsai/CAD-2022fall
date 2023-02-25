all: Lab02

CXX = g++ -std=c++11

LINKFLAGS = -Wall

Lab02: main.o Timing_Analyzer.o
	$(CXX) $(LINKFLAGS) -o 311510141.o main.o Timing_Analyzer.o

main.o: 311510141.cpp
	$(CXX) $(LINKFLAGS) -o main.o -c 311510141.cpp 

Timing_Analyzer.o: Timing_Analyzer.cpp Timing_Analyzer.h
	$(CXX) $(LINKFLAGS) -c Timing_Analyzer.cpp

clean:
	rm -rf *.o Lab02