CXX = g++
CXXFLAGS = -std=c++17 -O2 -Wall

SRCS = main.cpp Index.cpp 
OBJS = $(SRCS:.cpp=.o)


TARGET = search

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $<

clean:
	rm -f $(OBJS) $(TARGET)
