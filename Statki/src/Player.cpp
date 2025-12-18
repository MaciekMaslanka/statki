#pragma once
#include <vector>
#include "../include/Player.h"

Player::Player(std::vector<Ship*> fleet, int movePoints, std::string name)
:fleet(fleet), movePoints(movePoints), isHisTurn(false), name(name)
{}

void Player::addShip(Ship* ship) {fleet.push_back(ship);}
//gettery
std::vector<Ship*> Player::getFleet() const {return fleet;}
int Player::getMovePoints() const {return movePoints;}
bool Player::getIsHisTurn() const {return isHisTurn;}
//settery
void Player::setMovePoints(int movePoints) {this->movePoints = movePoints;}
void Player::setIsHisTurn(bool isHisTurn) {this->isHisTurn = isHisTurn;}