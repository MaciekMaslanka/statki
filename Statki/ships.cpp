#include "ships.h"
#include <array>

using namespace std;

//statek
Ship::Ship(array<int, 3> position, int length, int fuelAmount, int detectionRange, float health)
:position(position), length(length), fuelAmount(fuelAmount), detectionRange(detectionRange), health(health), isAlive(true)
{}
void Ship::shoot()
{
}
void Ship::move()
{
}
void Ship::takeDamage(float amount)
{
}
//gettery
array<int, 3> Ship::getPosition() const {return position;}
int Ship::getLength() const {return length;}
int Ship::getFuelAmount() const {return fuelAmount;}
float Ship::getHealth() const {return health;}
bool Ship::getIsAlive() const {return isAlive;}

//settery
void setPosition();
void setLength();
void setFuelAmount();



