
src = $(wildcard *.cpp)
exe = $(patsubst %.cpp,%,$(src))
CXXFLAGS = -std=c++11 -pthread

all: $(exe)
	echo done building $(exe)
