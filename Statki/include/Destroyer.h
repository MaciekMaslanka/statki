#pragma once
#include "Ship.h"

class Destroyer : public Ship
{
    protected:
        int torpedoesAmount;
        int initialTorpedoesAmount;
    public:
    Destroyer(std::array<int, 2> position, shipSize size, int fuelAmount, int detectionRange, float health, bool isAlive = true, int torpedoesAmount=0);
    bool canShoot() const override;
    bool canTorpedoAttack() const override;

    char getSymbol() const override;
};