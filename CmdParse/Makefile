.PHONY: all clean

SRC=$(wildcard *.cpp)
OBJS=$(SRC:.cpp=.o)

all:    ${OBJS}

%.o: %.cpp
	${CXX} -c $< -o $@ ${INCLUDE} ${CXXFLAGS}

clean:
	rm -rf *.o