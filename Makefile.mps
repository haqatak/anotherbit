# Makefile for building the MPS backend on macOS

# Compiler
CXX=clang++

# Directories
CUR_DIR=$(shell pwd)
LIBDIR=$(CUR_DIR)/lib
BINDIR=$(CUR_DIR)/bin

# Source directories
DIRS=util AddressUtil CmdParse CryptoUtil KeyFinderLib MpsKeySearchDevice secp256k1lib Logger

# Include paths
INCLUDE = $(foreach d, $(DIRS), -I$(CUR_DIR)/$d)
INCLUDE += -I/opt/homebrew/opt/openssl/include

# Libtorch paths
LIBTORCH_HOME?=/Users/haq/miniconda3/lib/python3.12/site-packages/torch
LIBTORCH_INCLUDE=${LIBTORCH_HOME}/include
LIBTORCH_LIB=${LIBTORCH_HOME}/lib
INCLUDE += -I${LIBTORCH_INCLUDE} -I${LIBTORCH_INCLUDE}/torch/csrc/api/include

# C++ flags
CXXFLAGS=-O2 -std=c++17 -Wno-unused-parameter

# Linker flags
LDFLAGS=-L/opt/homebrew/opt/openssl/lib -L${LIBDIR} -L${LIBTORCH_LIB} -rpath ${LIBTORCH_LIB}

# Libraries
LIBS=-lkeyfinder -laddressutil -lsecp256k1 -lcryptoutil -lMpsKeySearchDevice -llogger -lutil -lcmdparse -ltorch -lc10 -lcrypto

# Targets
all: mpsBitCrack

# Build the libraries
.SECONDEXPANSION:
lib%.a: $$(addprefix $$*, $$(notdir $$@))
	ar rvs $$@ $$?

libs: $(addprefix $(LIBDIR)/, libutil.a libaddressutil.a libcmdparse.a libcryptoutil.a libkeyfinder.a libMpsKeySearchDevice.a libsecp256k1.a liblogger.a)

$(LIBDIR)/libutil.a: $(wildcard util/*.cpp)
$(LIBDIR)/libaddressutil.a: $(wildcard AddressUtil/*.cpp)
$(LIBDIR)/libcmdparse.a: $(wildcard CmdParse/*.cpp)
$(LIBDIR)/libcryptoutil.a: $(wildcard CryptoUtil/*.cpp)
$(LIBDIR)/libkeyfinder.a: $(wildcard KeyFinderLib/*.cpp)
$(LIBDIR)/libMpsKeySearchDevice.a: $(wildcard MpsKeySearchDevice/*.cpp)
$(LIBDIR)/libsecp256k1.a: $(wildcard secp256k1lib/*.cpp)
$(LIBDIR)/liblogger.a: $(wildcard Logger/*.cpp)

%.o: %.cpp
	$(CXX) -c $< -o $@ $(CXXFLAGS) $(INCLUDE)

# Build the executable
mpsBitCrack: libs
	${CXX} -o ${BINDIR}/mpsBitCrack KeyFinder/main.cpp KeyFinder/ConfigFile.cpp KeyFinder/DeviceManager.cpp ${INCLUDE} ${CXXFLAGS} ${LDFLAGS} ${LIBS}

# Clean
clean:
	make --directory=util clean
	make --directory=AddressUtil clean
	make --directory=CmdParse clean
	make --directory=CryptoUtil clean
	make --directory=KeyFinderLib clean
	make --directory=MpsKeySearchDevice clean
	make --directory=secp256k1lib clean
	make --directory=Logger clean
	rm -rf ${LIBDIR}
	rm -rf ${BINDIR}

.PHONY: all libs clean
