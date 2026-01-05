#include "../include/AircraftCarrier.h"

using namespace std;

AircraftCarrier::AircraftCarrier(array<int, 2> position, shipType type, int fuelAmount, int detectionRange, float health, bool isAlive, 
    int aircraftAmount, int airStrikeDamage)
:Ship(position, type, fuelAmount, detectionRange, health, isAlive), 
aircraftAmount(aircraftAmount), initialAircraftAmount(aircraftAmount), airStrikeDamage(airStrikeDamage)
{}

bool AircraftCarrier::canAirStrike() const {return true;}
bool AircraftCarrier::tryAirStrike(Ship* target)
{
    int distanceToTarget = calculateDistance(position, target->getPosition());
    if (aircraftAmount > 0 && distanceToTarget <= detectionRange && !target->isStealth())
    {
        aircraftAmount - 10;
        target->takeDamage(airStrikeDamage);
        return true;
    }
    else
    {
        return false;
    }
}

//gettery
int AircraftCarrier::getAircraftAmount() const {return aircraftAmount;}
int AircraftCarrier::getInitialAircraftAmount() const {return initialAircraftAmount;}
char AircraftCarrier::getSymbol() const {return 'A';}
//settery
void AircraftCarrier::setAircraftAmount(int aircraftAmount) {this->aircraftAmount = aircraftAmount;}