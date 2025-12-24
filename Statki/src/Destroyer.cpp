#include "../include/Destroyer.h"

Destroyer::Destroyer(std::array<int, 2> position, shipSize size, shipType type, int fuelAmount, int detectionRange, float health, bool isAlive, int torpedoesAmount)
:Ship(position, size, type, fuelAmount, detectionRange, health, isAlive), torpedoesAmount(torpedoesAmount), initialTorpedoesAmount(torpedoesAmount)
{}
bool Destroyer::canShoot() const {return true;}
bool Destroyer::canTorpedoAttack() const {return true;}
char Destroyer::getSymbol() const {return 'D';}