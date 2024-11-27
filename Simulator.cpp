#include "Simulator.h"

Simulator::Simulator(int numFloors, int numElevators) 
{
    // Create all floor and elevator objects
    for (int i = 0; i < numElevators; ++i) {
        elevatorList.push_back(std::make_shared<Elevator>(i));
    }
    for (int i = 0; i < numFloors; ++i) {
        floorList.push_back(std::make_shared<Floor>(i));
    }
}

bool Simulator::allPassengersArrived()
{
    for (const auto& p : passengerList) {
        if (!p->arrived) {
            return false;
        }
    }
    return true;
}

void Simulator::loadData(const std::string& fileName)
{
    std::ifstream file(fileName);
    if (!file.is_open()) {
        std::cout << "Error: Could not open file: " << fileName << std::endl;
        return;
    }

    std::string line;
    int id = 0;

    // Skip the header
    std::getline(file, line);

    // Read the rest of the file
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string start_time_str, start_floor_str, end_floor_str;

        // Read each value by comma
        std::getline(ss, start_time_str, ',');
        std::getline(ss, start_floor_str, ',');
        std::getline(ss, end_floor_str, ',');

        // Convert strings to integers
        int start_time = std::stoi(start_time_str);
        int start_floor = std::stoi(start_floor_str);
        int end_floor = std::stoi(end_floor_str);

        // Create passenger and add to lists
        auto passenger = std::make_shared<Passenger>(id++, start_time, start_floor, end_floor);
        passengerList.push_back(passenger);
        floorList[start_floor]->addPassenger(passenger);
    }
}

void Simulator::run()
{
    while (true) {
        // Assign elevators to waiting passengers. Simple logic, balance passengers across elevators
        for (auto& passenger : passengerList) {
            if (passenger->startTime <= sysTime && !passenger->assignedToElevator) {
                // Find the closest elevator to the passenger's starting floor
                std::shared_ptr<Elevator> selectedElevator = nullptr;
                int minimumPassengers = std::numeric_limits<int>::max(); // Set an initial large value

                for (const auto& elevator : elevatorList) {
                    if (elevator->pickupRequests.size() < minimumPassengers) {
                        selectedElevator = elevator;
                        minimumPassengers = elevator->pickupRequests.size();
                    }               
                }

                // Add the passenger's start floor as a request for the closest non-full elevator
                selectedElevator->addPickupRequest(passenger->startFloor);
                passenger->assignedToElevator = true;        
            }           
        }

        // Update each elevator
        for (auto& elevator : elevatorList) {
            elevator->updateStatus(floorList, sysTime);
        }

        sysTime++;
        if (allPassengersArrived()) {
            return;
        }
    }
}

double Simulator::calculateAvgWaitTime()
{
    double totalWaitTime = 0;
    for (auto& p : passengerList) {
        totalWaitTime += p->waitTime;
    }
    return totalWaitTime / passengerList.size();
}

double Simulator::calculateAvgTravelTime()
{
    double totalTravelTime = 0;
    for (const auto& p : passengerList) {
        totalTravelTime += p->travelTime;
    }
    return totalTravelTime / passengerList.size();
}
