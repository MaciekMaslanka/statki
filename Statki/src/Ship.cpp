#include <vector>
#include <cmath>

#include "../include/Ship.h"
using namespace std;

int Ship::calculateDistance(array<int, 2> from, array<int, 2> to)
{
    int distance = abs(to[0] - from[0]) + abs(to[1] - from[1]);
    return distance;
}

Ship::Ship(array<int, 2> position, shipSize size, shipType type, int fuelAmount, int detectionRange, float health, bool isAlive)
:position(position), size(size), type(type), fuelAmount(fuelAmount), detectionRange(detectionRange), 
health(health), isAlive(isAlive), initialFuelAmount(fuelAmount), initialHealth(health)
{}

bool Ship::shootAttack(Ship& target) {return false;}
bool Ship::torpedoAttack(Ship& target) {return false;}
bool Ship::airStrike(Ship& target) {return false;}

bool Ship::canShoot() const {return false;}
bool Ship::canTorpedoAttack() const {return false;}
bool Ship::canAirStrike() const {return false;}

bool Ship::isStealth() const {return false;}
bool Ship::toogleDive() {return false;}

void Ship::move(array<int, 2> targetPosition)
{
    int distance = calculateDistance(position, targetPosition);
    if (distance <= fuelAmount)
    {
        position = targetPosition;
        fuelAmount -= distance;
    }
    else
    {
        cout<<"Cel jest za daleko"<<endl;
    }
    //TO DO: przerobić to żeby nie było couta
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
shipSize Ship::getSize() const {return size;}
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