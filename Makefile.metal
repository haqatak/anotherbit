# Makefile for building the Metal backend on macOS

# Compiler
CXX=clang++

# Directories
CUR_DIR=$(shell pwd)
LIBDIR=$(CUR_DIR)/lib
BINDIR=$(CUR_DIR)/bin

# Source directories
DIRS=util AddressUtil CmdParse CryptoUtil KeyFinderLib MetalKeySearchDevice secp256k1lib Logger

# Include paths
INCLUDE = $(foreach d, $(DIRS), -I$(CUR_DIR)/$d)
INCLUDE += -I/opt/homebrew/opt/openssl/include

# C++ flags
CXXFLAGS=-O2 -std=c++17 -Wno-unused-parameter -DBUILD_METAL

# Linker flags
LDFLAGS=-L/opt/homebrew/opt/openssl/lib -L${LIBDIR} -framework Metal -framework Foundation

# Libraries
LIBS=-lkeyfinder -laddressutil -lsecp256k1 -lcryptoutil -lMetalKeySearchDevice -llogger -lutil -lcmdparse -lcrypto

# Source files
SRCS = $(wildcard util/*.cpp) \
       $(wildcard AddressUtil/*.cpp) \
       $(wildcard CmdParse/*.cpp) \
       $(wildcard CryptoUtil/*.cpp) \
       $(wildcard KeyFinderLib/*.cpp) \
       $(wildcard MetalKeySearchDevice/*.cpp) \
       $(wildcard secp256k1lib/*.cpp) \
       $(wildcard Logger/*.cpp) \
       KeyFinder/main.cpp \
       KeyFinder/ConfigFile.cpp \
       KeyFinder/DeviceManager.cpp

# Target
all: metalBitCrack

metalBitCrack:
	mkdir -p ${BINDIR}
	${CXX} -o ${BINDIR}/metalBitCrack ${SRCS} ${INCLUDE} ${CXXFLAGS} ${LDFLAGS} ${LIBS}

clean:
	rm -rf ${BINDIR}

.PHONY: all clean
