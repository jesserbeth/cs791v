# all: fireSim

# fireSim: simulation.cpp fireSim.h fireSim.cpp FuelModel.h FuelMoisture.h
# 	g++ -g - Wall -o fireSim simulation.cpp fireSim.h fireSim.cpp FuelModel.h FuelMoisture.h

# clean:
# 	rm -f *.o
# 	rm -f *~

FILES= simulation.cu kernel.cu fireSim.h fireSim.cpp FuelModel.h FuelMoisture.h
CXXFLAGS= -g -o 
SOURCE= fireSim
INCLUDES= -I /usr/include/gdal/
LIBS=  -L/usr/lib -lgdal
# CC=g++
CC= nvcc

all: fireSim

fireSim: simulation.cu fireSim.cpp 
	$(CC) $^  $(CXXFLAGS) $@ $(LIBS) $(INCLUDES) -arch=sm_20

clean:
	rm -f *.o
	rm -f *~



fireSim.o: 