#pragma once

#include <memory>
#include <queue>
#include "Passenger.h"

class Floor {
public:

	// Floor attributes
	int floorNumber;
	std::queue<std::shared_ptr<Passenger>> waitingPassengers;

	// Floor Methods
	Floor(int floorNumberIn);
	void addPassenger(std::shared_ptr<Passenger> passenger);

};