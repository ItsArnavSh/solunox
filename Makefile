CXX      = g++
CXXFLAGS = -Wall -Iinclude
TARGET   = solunox
SRCS     = $(wildcard src/*.cpp)
OBJS     = $(SRCS:.cpp=.o)

all: build

build: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: build
	./$(TARGET) examples/hello.slx

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all build run clean
