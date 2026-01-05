#include "../include/Destroyer.h"

Destroyer::Destroyer(std::array<int, 2> position, shipType type, int fuelAmount, int detectionRange, float health, bool isAlive, 
    int torpedoesAmount, int shootAttackDmg, int torpedoAttackDmg)
:Ship(position, type, fuelAmount, detectionRange, health, isAlive), 
torpedoesAmount(torpedoesAmount), initialTorpedoesAmount(torpedoesAmount), shootAttackDmg(shootAttackDmg), torpedoAttackDmg(torpedoAttackDmg)
{}
bool Destroyer::canShoot() const {return true;}
bool Destroyer::tryShootAttack(Ship* target)
{
    int distanceToTarget = calculateDistance(position, target->getPosition());
    if (distanceToTarget <= detectionRange && !target->isStealth())
    {
        target->takeDamage(shootAttackDmg);
        return true;
    }
    else
    {
        return false;
    }
}

bool Destroyer::canTorpedoAttack() const {return true;}
bool Destroyer::tryTorpedoAttack(Ship* target)
{
    int distanceToTarget = calculateDistance(position, target->getPosition());
    if (distanceToTarget <= detectionRange && !target->isStealth() && torpedoesAmount > 0)
    {
        target->takeDamage(torpedoAttackDmg);
        torpedoesAmount--;
        return true;
    }
    else
    {
        return false;
    }
}

int Destroyer::getShootAttackDmg() const {return shootAttackDmg;}
int Destroyer::getTorpedoAttackDmg() const {return torpedoAttackDmg;}
int Destroyer::getTorpedoesAmount() const {return torpedoesAmount;}
int Destroyer::getInitialTorpedoesAmount() const {return initialTorpedoesAmount;}

char Destroyer::getSymbol() const {return 'D';}