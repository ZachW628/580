CXX=g++
CXXFLAGS= -std=c++11 -g -c


main: graph.o
	$(CXX) graph.o -o graph

graph.o: graph.cpp
	$(CXX) $(CXXFLAGS) graph.cpp

clean:
	rm -rf graph *.o
