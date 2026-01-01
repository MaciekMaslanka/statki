#include "../include/Cruiser.h"

Cruiser::Cruiser(std::array<int, 2> position, shipType type, int fuelAmount, int detectionRange, float health, bool isAlive)
:Ship(position, type, fuelAmount, detectionRange, health, isAlive)
{
}
char Cruiser::getSymbol() const {return 'C';}
bool Cruiser::canShoot() const {return true;}