#include "../include/AircraftCarrier.h"

using namespace std;

AircraftCarrier::AircraftCarrier(array<int, 2> position, shipSize size, int fuelAmount, int detectionRange, float health, bool isAlive, int aircraftAmount)
:Ship(position, size, fuelAmount, detectionRange, health, isAlive), aircraftAmount(aircraftAmount), initialAircraftAmount(aircraftAmount)
{}

bool AircraftCarrier::canAirStrike() const {return true;}

//gettery
int AircraftCarrier::getAircraftAmount() const {return aircraftAmount;}
char AircraftCarrier::getSymbol() const {return 'A';}
//settery
void AircraftCarrier::setAircraftAmount(int aircraftAmount) {this->aircraftAmount = aircraftAmount;}