
src = $(wildcard *.cpp)
exe = $(patsubst %.cpp,%,$(src))
CXXFLAGS = -O3 -std=c++14 -pthread

all: $(exe)

clean:
	rm -f $(exe)
