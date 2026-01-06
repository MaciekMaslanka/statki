#include <vector>
#include "../include/Player.h"

Player::Player(std::vector<Ship*> fleet, int movePoints, std::string name)
:fleet(fleet), movePoints(movePoints), name(name)
{}

void Player::addShip(Ship* ship) {fleet.push_back(ship);}
void Player::useMovePoint()
{
    movePoints--;
}

//gettery
std::vector<Ship*> Player::getFleet() const {return fleet;}
int Player::getMovePoints() const {return movePoints;}
std::string Player::getName() const {return name;}
//settery
void Player::setMovePoints(int movePoints) {this->movePoints = movePoints;}

Player::~Player()
{
    for (Ship* ship : fleet)
    {
        delete ship;
    }
    fleet.clear();
}