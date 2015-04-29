# all: fireSim

# fireSim: simulation.cpp fireSim.h fireSim.cpp FuelModel.h FuelMoisture.h
# 	g++ -g - Wall -o fireSim simulation.cpp fireSim.h fireSim.cpp FuelModel.h FuelMoisture.h

# clean:
# 	rm -f *.o
# 	rm -f *~

FILES= simulation.cu kernel.cu fireSim.h fireSim.cpp FuelModel.h FuelMoisture.h
CXXFLAGS= -g -arch=compute_20 -o
SOURCE= fireSim
INCLUDES= -I /usr/include/gdal/
LIBS=  -L/usr/lib -lgdal
CC= nvcc

all: fireSim

fireSim: simulation.cu fireSim.cpp 
	$(CC) --ptxas-options=-v $^  $(CXXFLAGS) $@ $(LIBS) $(INCLUDES) 

clean:
	rm -f *.o
	rm -f *~
