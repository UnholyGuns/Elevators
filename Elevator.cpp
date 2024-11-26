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
    if (status == STOPPING) {
        if (stopTimer > 0) {
            stopTimer--;
            if (stopTimer == 0) {
                status = STOPPED;
            }
            return;
        }
    }
    
    if (status == STOPPED) {
        auto& floor = floors[currentFloor];

        // Discharge passengers
        std::vector<std::shared_ptr<Passenger>> tempRemainingPassengers;

        for (auto& passenger : this->passengers) {
            if (passenger->endFloor == currentFloor) {
                passenger->travelTime = time - passenger->startTime - passenger->waitTime;
                passenger->arrived = true;
            }
            else {
                tempRemainingPassengers.push_back(passenger);
            }
        }

        this->passengers = std::move(tempRemainingPassengers);

        // Load passengers that are present wrt time
        // check all waiting passengers if its time to board
        int initialQueueSize = floor->waitingPassengers.size();

        while (initialQueueSize > 0 && passengers.size() < MAX_CAPACITY) {
            auto passenger = floor->waitingPassengers.front();

            // Check that the passengers assigned to the floor are present at the current instance in time
            if (passenger->startTime <= time) {
                floor->waitingPassengers.pop();

                this->passengers.push_back(passenger);
                passenger->waitTime = time - passenger->startTime;
                addRequest(passenger->endFloor);
                pickupRequestCnt--;
            }else {
                // If not ready ready, move to the back of the queue
                floor->waitingPassengers.pop();
                floor->waitingPassengers.push(passenger);
            }

            initialQueueSize--;
        }

        // Determine next state
        

    }
}

void Elevator::updateStatus()
{
    if (status == MOVING_UP || status == MOVING_DOWN) {
       // Can we pick anyone up on the way?
        if (pickupRequests.count(currentFloor) > 0 && passengers.size() < MAX_CAPACITY) {
            // Found a floor request; stop here
            status = STOPPING;
            stopTimer = STOPPING_TIME;
        }

        // Can we drop anyone off on the way?
        if (dropoffRequests.count(currentFloor) > 0) {
            // Found a floor request; stop here
            status = STOPPING;
            stopTimer = STOPPING_TIME;
        }
    }

    if (status == STOPPED) {
        int targetFloor = *floorRequests.begin();
        if (!floorRequests.empty() && currentFloor != targetFloor) {
            if (targetFloor > currentFloor) {
                status = MOVING_UP;
            }
            else if (targetFloor < currentFloor) {
                status = MOVING_DOWN;
            }
        }
    }
}

void Elevator::addPickupRequest(int floor)
{
    pickupRequests.insert(floor);
}

void Elevator::addDroppoffRequest(int floor)
{
    dropoffRequests.insert(floor);
}

