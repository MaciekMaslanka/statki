#pragma once
#include "Ship.h"

class Destroyer : public Ship
{
    protected:
        int torpedoesAmount;
        int initialTorpedoesAmount;
        int shootAttackDmg;
        int torpedoAttackDmg;
    public:
    Destroyer(std::array<int, 2> position, shipType type, int fuelAmount, int detectionRange, float health, bool isAlive = true, 
        int torpedoesAmount=0, int shootAttackDmg=5, int torpedoAttackDmg=5);

    bool canShoot() const override;
    bool tryShootAttack(Ship* target) override;
    bool canTorpedoAttack() const override;
    bool tryTorpedoAttack(Ship* target) override;

    int getShootAttackDmg() const;
    int getTorpedoAttackDmg() const;

    char getSymbol() const override;
};