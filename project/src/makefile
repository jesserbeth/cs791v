all: fireSim

fireSim: simulation.cpp fireSim.h fireSim.cpp FuelModel.h FuelMoisture.h
	g++ -o fireSim simulation.cpp fireSim.h fireSim.cpp FuelModel.h FuelMoisture.h

clean:
	rm -f *.o
	rm -f *~
