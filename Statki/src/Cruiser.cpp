#include "../include/Cruiser.h"

Cruiser::Cruiser(std::array<int, 2> position, shipSize size, int fuelAmount, int detectionRange, float health, bool isAlive)
:Ship(position, size, fuelAmount, detectionRange, health, isAlive)
{
}
char Cruiser::getSymbol() const {return 'C';}
bool Cruiser::canShoot() const {return true;}