CXXFLAGS =	-std=c++11 -O2 -g -Wall -fmessage-length=0

CPP_FILES := $(wildcard src/*.cpp)

OBJS := $(addprefix obj/,$(notdir $(CPP_FILES:.cpp=.o)))

LIBS = -lboost_graph

TARGET =	Docgraph

################


obj/%.o: src/%.cpp
	g++ $(CXXFLAGS) -c -o $@ $<
   
$(TARGET):	$(OBJS)
	$(CXX) -o $(TARGET) $(OBJS) $(LIBS)

%.main: obj/docgraph_impl.o obj/%.o
	$(CXX) -o $@ $(addprefix obj/,$(@:.main=.o)) obj/docgraph_impl.o $(LIBS)

all:	$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)

