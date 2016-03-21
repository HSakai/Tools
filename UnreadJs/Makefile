TARGET = exe/UnreadJs

CC = g++
CXXFLAGS = -g -std=c++11 -Wall
SRCS = $(shell find ./src -type f -name "*.cpp")
OBJS = $(SRCS:%.cpp=%.o)
LIB = -lboost_filesystem -lboost_regex -lboost_system

$(TARGET):$(OBJS)
	$(RM) $@
	$(CC) $(LIB) $(CXXFLAGS) -o $@ $^
	$(RM) $(OBJS)

%.o:%.cpp
	$(CC) -o $@ -c $<
