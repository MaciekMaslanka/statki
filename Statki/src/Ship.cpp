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

bool Ship::canShoot() const {return false;}
bool Ship::canTorpedoAttack() const {return false;}
bool Ship::canAirStrike() const {return false;}

bool Ship::isStealth() const {return false;}
bool Ship::canDive() const {return false;}
bool Ship::toogleDive() {return false;}

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
        sink();
    }
}

void Ship::sink()
{
    isAlive = false;
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