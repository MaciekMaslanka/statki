#pragma once
#include "Ship.h"

class Submarine : public Ship
{
    protected:
        bool isUnderwater;
        int turnsUnderwater;
        int torpedoesAmount;
        int initialTorpedoesAmount;

    public:
    Submarine(std::array<int, 2> position, shipSize size, int fuelAmount, int detectionRange, float health, bool isAlive=true, int torpedoesAmount=0);

    bool canTorpedoAttack() const override;
    bool isStealth() const override;
    bool toogleDive() override;
    
    //gettery
    bool getIsUnderwater() const;
    int getTurnsUnderwater() const;
    int getTorpedoesAmount() const;
    char getSymbol() const override;
    //settery
    void setIsUnderwater(bool isUnderwater);
    void setTurnsUnderwater(int turnsUnderwater);
    void setTorpedoesAmount(int torpedoesAmount);

     ~Submarine() = default;
};