#pragma once
#include <vector>
#include "Ship.h"

class Player
{
    protected:
        std::vector<Ship*> fleet;
        std::string name;
        int movePoints;
        bool isHisTurn;
    public:
        Player(std::vector<Ship*> fleet, int movePoints, std::string name="");
        
        void addShip(Ship* ship);

        //gettery
        std::vector<Ship*> getFleet() const;
        int getMovePoints() const;
        bool getIsHisTurn() const;
        //settery
        void setMovePoints(int movePoints);
        void setIsHisTurn(bool isHisTurn);
        ~Player() = default;
};