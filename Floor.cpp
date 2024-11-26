#include "Floor.h"

Floor::Floor(int floorNumberIn) : floorNumber(floorNumberIn) {}

void Floor::addPassenger(std::shared_ptr<Passenger> passenger)
{
	waitingPassengers.push(passenger);
}
