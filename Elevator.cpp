#include "Elevator.h"

Elevator::Elevator(int idIn) : ID(idIn) {}

void Elevator::move()
{
	if (MOVING_UP == status) {
		currentFloor++;
	}else if (MOVING_DOWN == status) {
		currentFloor--;
	}
}

void Elevator::stop(std::vector<std::shared_ptr<Floor>>& floors, int time)
{       
        auto& floor = floors[currentFloor];

        // Discharge passengers
        std::vector<std::shared_ptr<Passenger>> tempRemainingPassengers;

        for (auto& passenger : this->passengers) {
            if (passenger->endFloor == currentFloor) {
                passenger->travelTime = time - passenger->startTime - passenger->waitTime;
                passenger->arrived = true;
                dropoffRequests.erase(currentFloor);
            }
            else {
                tempRemainingPassengers.push_back(passenger);
            }
        }

        this->passengers = std::move(tempRemainingPassengers);

        // Load passengers that are present wrt time
        // check all waiting passengers if its time to board
        int initialQueueSize = floor->waitingPassengers.size();

        if (initialQueueSize == 0) {
            pickupRequests.erase(currentFloor);
        }

        while (initialQueueSize > 0) {
            auto passenger = floor->waitingPassengers.front();

            // Check that the passengers assigned to the floor are present at the current instance in time
            if (passenger->startTime <= time) {
                if (passengers.size() < MAX_CAPACITY) {
                    floor->waitingPassengers.pop();
                    this->passengers.push_back(passenger);
                    passenger->waitTime = time - passenger->startTime;
                    addDropoffRequest(passenger->endFloor);
                    pickupRequests.erase(currentFloor);
                }
                else {
                    passenger->assignedToElevator = false;// Tell the simulator there are more passengers here to pickup
                }

            }else {
                // If not ready ready, move to the back of the queue
                floor->waitingPassengers.pop();
                floor->waitingPassengers.push(passenger);
            }
            initialQueueSize--;
        }      
    
}

void Elevator::updateStatus(std::vector<std::shared_ptr<Floor>>& floors, int time)
{
    switch (status) {
    case STOPPING:
        if (stopTimer > 0) {
            stopTimer--;
            if (stopTimer == 0) {
                status = STOPPED;
            }
            return;
        }
        break;
    case MOVING_UP:
    case MOVING_DOWN:

        if (movingTimer > 0) {
            movingTimer--;
            return;
        }

        move();

        // Can we pick anyone up on the way?
        if (pickupRequests.count(currentFloor) > 0 && passengers.size() < MAX_CAPACITY) {
            // Found a floor request and can fit passengers, stop here
            status = STOPPING;
            stopTimer = STOPPING_TIME;
        }

        // Can we drop anyone off on the way?
        if (dropoffRequests.count(currentFloor) > 0) {
            // Found a dropoff request; stop here
            status = STOPPING;
            stopTimer = STOPPING_TIME;
        }

        if (targetFloor == currentFloor) {
            status = STOPPING;
            stopTimer = STOPPING_TIME;
        }
        break;
    case STOPPED:

        stop(floors, time);// Discharge/load passengers based on current time and floor

        // Determine what to do next
        if (targetFloor == currentFloor) {
            if (!pickupRequests.empty() && passengers.size() < MAX_CAPACITY) {
                targetFloor = *pickupRequests.begin();
            }
            else if (!dropoffRequests.empty()) {
                targetFloor = *dropoffRequests.begin();
            }
        }
        
        if (targetFloor > currentFloor) {
            status = MOVING_UP;
            movingTimer = MOVING_TIME_PER_FLOOR;
        }
        else if (targetFloor < currentFloor) {
            status = MOVING_DOWN;
            movingTimer = MOVING_TIME_PER_FLOOR;
        }
    }
}

void Elevator::addPickupRequest(int floor)
{
    pickupRequests.insert(floor);
}

void Elevator::addDropoffRequest(int floor)
{
    dropoffRequests.insert(floor);
}

