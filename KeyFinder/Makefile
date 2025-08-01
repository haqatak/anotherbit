CPPSRC:=$(wildcard *.cpp)

all:
ifeq ($(BUILD_CUDA), 1)
	${CXX} -DBUILD_CUDA -o cuKeyFinder.bin ${CPPSRC} ${INCLUDE} -I${CUDA_INCLUDE} ${CXXFLAGS} ${LIBS} -L${CUDA_LIB} -lkeyfinder -laddressutil -lsecp256k1 -lcryptoutil -lsecp256k1 -lcudautil -llogger -lutil -lCudaKeySearchDevice -lcudadevrt -lcudart -lcmdparse
	mkdir -p $(BINDIR)
	cp cuKeyFinder.bin $(BINDIR)/cuBitCrack
endif
ifeq ($(BUILD_OPENCL),1)
	${CXX} -DBUILD_OPENCL -o clKeyFinder.bin ${CPPSRC} ${INCLUDE} -I${OPENCL_INCLUDE} ${CXXFLAGS} ${LDFLAGS} ${LIBS} -L${OPENCL_LIB} -lkeyfinder -laddressutil -lsecp256k1 -lcryptoutil -lsecp256k1 -lCLKeySearchDevice -lclutil -llogger -lutil -lcmdparse
	mkdir -p $(BINDIR)
	cp clKeyFinder.bin $(BINDIR)/clBitCrack
endif
ifeq ($(BUILD_MPS),1)
	${CXX} -DBUILD_MPS -o mpsKeyFinder.bin ${CPPSRC} ${INCLUDE} ${CXXFLAGS} ${LDFLAGS} ${LIBS} -lkeyfinder -laddressutil -lsecp256k1 -lcryptoutil -lsecp256k1 -lMpsKeySearchDevice -llogger -lutil -lcmdparse -ltorch -lc10
	mkdir -p $(BINDIR)
	cp mpsKeyFinder.bin $(BINDIR)/mpsBitCrack
endif

clean:
	rm -rf cuKeyFinder.bin
	rm -rf clKeyFinder.bin
	rm -rf mpsKeyFinder.bin
