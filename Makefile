CC = gcc
CXX = g++

CFLAGS = -O3 -Intl/include
CXXFLAGS = $(CFLAGS)

SOURCES = reed-solomon.cc gf2eutil.cc rsencoder.cc rsexception.cc berlekamp-massey.cc rsfencoder.cc rsfdecoder.cc
OBJECTS = reed-solomon.o gf2eutil.o rsencoder.o rsexception.o berlekamp-massey.o rsfencoder.o rsfdecoder.o
TARGETS = reed-solomon rsfencoder rsfdecoder

all: $(TARGETS)

run: reed-solomon
	./reed-solomon

clean-objs:
	rm -f $(OBJECTS)

clean-targets:
	rm -f $(TARGETS)

clean: clean-objs clean-targets

.cc.o: $(SOURCES)
	$(CXX) $(CXXFLAGS) -c $< -o $@
	
reed-solomon: reed-solomon.o gf2eutil.o rsencoder.o rsexception.o berlekamp-massey.o
	$(CXX) $(CXXFLAGS) $^ -o $@ ntl/ntl.a

rsfencoder: rsfencoder.o gf2eutil.o rsencoder.o rsexception.o berlekamp-massey.o
	$(CXX) $(CXXFLAGS) $^ -o $@ ntl/ntl.a

rsfdecoder: rsfdecoder.o gf2eutil.o rsencoder.o rsexception.o berlekamp-massey.o
	$(CXX) $(CXXFLAGS) $^ -o $@ ntl/ntl.a

