#include "../include/Cruiser.h"

Cruiser::Cruiser(std::array<int, 2> position, shipSize size, shipType type, int fuelAmount, int detectionRange, float health, bool isAlive)
:Ship(position, size, type, fuelAmount, detectionRange, health, isAlive)
{
}
char Cruiser::getSymbol() const {return 'C';}
bool Cruiser::canShoot() const {return true;}