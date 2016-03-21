TARGET = UnreadJs

CC = g++
CXXFLAGS = -g -std=c++11 -Wall
SRCS = $(shell find ./src -type f -name "*.cpp")
OBJS = $(SRCS:%.cpp=%.o)

$(TARGET):$(OBJS)
	$(CC) $(CXXFLAGS) -o $@ $^

%.o:%.cpp
	$(CC) -o $@ -c $<
