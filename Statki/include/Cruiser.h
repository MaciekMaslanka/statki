#pragma once
#include "Ship.h"

class Cruiser : public Ship
{
    private:
        int shootAttackDmg;
    public:
    Cruiser(std::array<int, 2> position, shipType type, int fuelAmount, int detectionRange, float health, bool isAlive = true, int shootAttackDmg=5);

    bool canShoot() const override;
    int getShootAttackDmg() const override;
    bool tryShootAttack(Ship* target) override;
    char getSymbol() const override;
};