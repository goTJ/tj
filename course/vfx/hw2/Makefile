CXXFLAGS= -Wall -ansi -O9
LIBS= -lgil2 -lIlmImf -lIex -lHalf -lgsl -lgslcblas -lpng12
CXXFLAGS+= -I/usr/include/libpng12 -I/home/student/92/b92106/pjc/fhc/vfx/lib/include
LIBS+= -L/home/student/92/b92106/pjc/fhc/vfx/lib/lib

all: sift

tonemapping: tonemapping.cpp
	$(CXX) $(CXXFLAGS) $(LIBS) tonemapping.cpp -o tonemapping

sift: sift.cpp
	$(CXX) $(CXXFLAGS) $(LIBS) sift.cpp -o sift

program: program.cpp FeatureSet.cpp FeatureSet.h share/Debugger.h
	$(CXX) $(CXXFLAGS) $(LIBS) -o program program.cpp FeatureSet.cpp -lANN -lgsl

clean:
	rm -rf sift sift.o

.SUFFIXES: .cpp
.PHONY: all
