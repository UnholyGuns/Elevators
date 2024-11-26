#pragma once

class Passenger {
public:

	// Passenger attributes
	int ID, startTime, startFloor, endFloor;
	int waitTime = 0;
	int travelTime = 0;
	bool arrived = false;
	bool assignedToElevator = false;

	// Passenger methods
	Passenger(int idIn, int startTimeIn, int startFloorIn, int endFloorIn);

};