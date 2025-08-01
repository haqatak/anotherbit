NAME=MpsKeySearchDevice
SRC=$(wildcard *.cpp)
OBJS=$(SRC:.cpp=.o)

all:    ${SRC}
	for file in ${SRC} ; do\
		${CXX} -c $$file ${INCLUDE} ${CXXFLAGS};\
	done
	ar rvs ${LIBDIR}/lib$(NAME).a ${OBJS}

clean:
	rm -rf *.o *.a
