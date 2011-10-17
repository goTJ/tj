CXX = g++
CLIB = -lgil2
CFLAG = -Wall -g

.PHONY: clean

shot_detect: shot_detect.cpp frame.o
	$(CXX) $(CFLAG) $(CLIB) shot_detect.cpp frame.o -o shot_detect

frame.o: frame.cpp
	$(CXX) $(CFLAG) -c frame.cpp

clean:
	rm -f shot_detect *.o
