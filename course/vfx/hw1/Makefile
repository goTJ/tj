LIBS= -lgil `pkg-config --libs gsl`
hdr: hdr.cpp
	g++ hdr.cpp ${LIBS} -o hdr

tonemapping: tonemapping.cpp
	g++ tonemapping.cpp -lgil2 -o tonemapping
