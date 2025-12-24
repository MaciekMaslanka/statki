#pragma once
#include "Ship.h"

class Cruiser : public Ship
{
    public:
    Cruiser(std::array<int, 2> position, shipSize size, shipType type, int fuelAmount, int detectionRange, float health, bool isAlive = true);

    bool canShoot() const override;
    char getSymbol() const override;
};