CXX = g++

CXXFLAGS = -std=c++11 -O2 -g -Wall -fmessage-length=0 -I/Users/juan/boost_library/include

CPP_FILES := $(wildcard src/*.cpp)

OBJS := $(addprefix obj/,$(notdir $(CPP_FILES:.cpp=.o)))

LIBS = -L/Users/juan/boost_library/lib -lboost_graph

TARGET = Docgraph

################


obj/%.o: src/%.cpp
	g++ $(CXXFLAGS) -c -o $@ $<
   
$(TARGET):	$(OBJS)
	$(CXX) -o $(TARGET) $(OBJS) $(LIBS)

%.main: obj/docgraph_impl.o obj/%.o
	$(CXX) -o $@ $(addprefix obj/,$(@:.main=.o)) obj/docgraph_impl.o $(LIBS)

all:	docgraph_build.main colgraph_build.main

clean:
	rm -f $(OBJS) $(TARGET) *.main

