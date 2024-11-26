#pragma once

#include <vector>
#include <memory>
#include <set>
#include "Passenger.h"
#include "Floor.h"

class Elevator {
public:

	// Enumeration for 
	// the elevator status
	enum ElevatorStatus {
		STOPPED = 0,
		STOPPING,
		MOVING_UP,
		MOVING_DOWN
	};

	// Enumeration for the elevator direction
	enum ElevatorDirection {
		UP = 0,
		DOWN
	};

	// Elevator attributes
	static const int MOVING_TIME_PER_FLOOR = 10;// Time to move one floor
	static const int STOPPING_TIME = 2;// Time to stop
	static const int MAX_CAPACITY = 8;

	int ID;
	int currentFloor = 0;
	int stopTimer = STOPPING_TIME;
	int movingTimer = 0;

	ElevatorStatus status = STOPPED;
	ElevatorDirection direction = UP;
	std::vector<std::shared_ptr<Passenger>> passengers;
	std::set<int> pickupRequests;// Maintains order and uniqness
	std::set<int> dropoffRequests;// Maintains order and uniqness

public:
	// Constructor
	Elevator(int idIn);

	// Elevator methods
	void move();
	void stop(std::vector<std::shared_ptr<Floor>>& floors, int time);
	void updateStatus();
	void addPickupRequest(int floor);
	void addDroppoffRequest(int floor);

};