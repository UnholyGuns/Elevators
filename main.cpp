#include <iostream>

#include "Elevator.h"
#include "Passenger.h"
#include "Floor.h"
#include "Simulator.h"

using namespace std;

int main(void) {
    int numElevators = 4;
    int numFloors = 100;

    Simulator sim(numFloors, numElevators);
    sim.loadData("C:\\Users\\fergutr1\\Documents\\MyStuff\\Masters\\Classes\\Fall 2024\\C++\\Elevators\\Mod10_Assignment_Elevators.csv");
    sim.run();

    std::cout << "Average Wait Time: " << sim.calculateAvgWaitTime() << " seconds\n";
    std::cout << "Average Travel Time: " << sim.calculateAvgTravelTime() << " seconds\n";

    return 0;
}