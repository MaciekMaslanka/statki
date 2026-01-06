#include "../include/Cruiser.h"

Cruiser::Cruiser(std::array<int, 2> position, shipType type, int fuelAmount, int detectionRange, float health, bool isAlive, int shootAttackDmg)
:Ship(position, type, fuelAmount, detectionRange, health, isAlive), shootAttackDmg(shootAttackDmg)
{
}
bool Cruiser::canShoot() const {return true;}
int Cruiser::getShootAttackDmg() const {return shootAttackDmg;}
bool Cruiser::tryShootAttack(Ship* target)
{
    int distanceToTarget = calculateDistance(position, target->getPosition());
    if (distanceToTarget <= detectionRange && !target->isStealth())
    {
        target->takeDamage(shootAttackDmg);
        hasAttackedThisTurn = true;
        return true;
    }
    else
    {
        return false;
    }
}
char Cruiser::getSymbol() const {return 'C';}
