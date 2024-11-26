#pragma once

#include "Elevator.h"
#include "Passenger.h"
#include "Floor.h"
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

// A class intended to run the simulation by updating the state of each object and keep track of simulation time

class Simulator {
private:

	// Simulation time
	int sysTime = 0;

	// Simulation object lists
	std::vector<std::shared_ptr<Elevator>> elevatorList;
	std::vector<std::shared_ptr<Passenger>> passengerList;
	std::vector<std::shared_ptr<Floor>> floorList;

public:
	// Constructor
	Simulator(int numFloors, int numElevators);

	// Simulation methods
	bool allPassengersArrived();
	void loadData(const std::string& fileName);
	void run();
	double calculateAvgWaitTime();
	double calculateAvgTravelTime();

};