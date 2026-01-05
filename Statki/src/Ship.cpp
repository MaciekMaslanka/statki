#include <vector>
#include <cmath>

#include "../include/Ship.h"
using namespace std;

int Ship::calculateDistance(array<int, 2> from, array<int, 2> to)
{
    int distance = abs(to[0] - from[0]) + abs(to[1] - from[1]);
    return distance;
}

Ship::Ship(array<int, 2> position, shipType type, int fuelAmount, int detectionRange, float health, bool isAlive)
:position(position), type(type), fuelAmount(fuelAmount), detectionRange(detectionRange), 
health(health), isAlive(isAlive), initialFuelAmount(fuelAmount), initialHealth(health)
{}
//ataki
bool Ship::canShoot() const {return false;}
bool Ship::canTorpedoAttack() const {return false;}
bool Ship::canAirStrike() const {return false;}

bool Ship::tryShootAttack(Ship* target) {return false;}
bool Ship::tryTorpedoAttack(Ship* target) {return false;}
bool Ship::tryAirStrike(Ship* target) {return false;}

int Ship::getShootAttackDmg() const {return 0;}

int Ship::getTorpedoAttackDmg() const {return 0;}
int Ship::getTorpedoesAmount() const {return 0;}
int Ship::getInitialTorpedoesAmount() const {return 0;} 

int Ship::getAirStrikeDmg() const {return 0;}
int Ship::getAircraftAmount() const {return 0;}
int Ship::getInitialAircraftAmount() const {return 0;}

bool Ship::isStealth() const {return false;}
bool Ship::canDive() const {return false;}
int Ship::getTurnsUnderwater() const {return 0;};
void Ship::enterStealth() {};
void Ship::leaveStealth() {};

void Ship::onNewTurn() {};
void Ship::move(array<int, 2> targetPosition)
{
    int distance = calculateDistance(position, targetPosition);
    fuelAmount -= distance;
    position = targetPosition;
}
int Ship::calculateMoveCost(array<int, 2> targetPosition)
{
    int distance = calculateDistance(position, targetPosition);

    if (distance <= detectionRange && distance <= fuelAmount)
    {
        return distance;
    }
    else
    {
        return -1;
    }
}

void Ship::takeDamage(float amount)
{
    health -= amount;
    if (health <= 0)
    {
        isAlive = false;
    }
}

//gettery
std::array<int, 2> Ship::getPosition() const {return position;}
shipType Ship::getType() const {return type;}
int Ship::getFuelAmount() const {return fuelAmount;}
int Ship::getInitialFuelAmount() const {return initialFuelAmount;}
float Ship::getHealth() const {return health;}
float Ship::getInitialHealth() const {return initialHealth;}
bool Ship::getIsAlive() const {return isAlive;}
int Ship::getDetectionRange() const {return detectionRange;}
char Ship::getSymbol() const {return '?';}
//settery
void Ship::setPosition(std::array<int, 2>targetPosition) {position = targetPosition;}
void Ship::setFuelAmount(int targetFuelAmount) {fuelAmount = targetFuelAmount;}