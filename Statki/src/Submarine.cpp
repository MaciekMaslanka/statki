#include "../include/Submarine.h"


    Submarine::Submarine(
        std::array<int, 2> position, shipType type, int fuelAmount, int detectionRange, float health, bool isAlive, 
        int torpedoesAmount)
    :Ship(position, type, fuelAmount, detectionRange, health, isAlive), torpedoesAmount(torpedoesAmount), 
    isUnderwater(false), turnsUnderwater(0), initialTorpedoesAmount(torpedoesAmount)
    {}

    bool Submarine::canTorpedoAttack() const 
    {
        if (isStealth() && torpedoesAmount <= 0)
        {
            return false;
        }
        else
        {
            return true;
        }
    }
    bool Submarine::isStealth() const {return isUnderwater && isAlive;}
    bool Submarine::canDive() const {return true;}

    void Submarine::enterStealth() 
    {
        if (!isUnderwater && turnsUnderwater > 0)
        {
            isUnderwater = true;
        }
    }
    void Submarine:: exitStealth()
    {
        isUnderwater = false;
    }
    
    void Submarine::onNewTurn()
    {
        if (!isStealth() && turnsUnderwater < maxTurnsUnderwater)
        {
            turnsUnderwater++;
        }
        if (isUnderwater)
        {
            turnsUnderwater--;
            if (turnsUnderwater <= 0)
            {
                exitStealth();
            }
        }
    }

    //gettery
    bool Submarine::getIsUnderwater() const {return isUnderwater;}
    int Submarine::getTurnsUnderwater() const {return turnsUnderwater;}
    int Submarine::getTorpedoesAmount() const {return torpedoesAmount;}
    char Submarine::getSymbol() const {return 'S';}
    //settery
    void Submarine::setIsUnderwater(bool isUnderwater) {this->isUnderwater = isUnderwater;}
    void Submarine::setTurnsUnderwater(int turnsUnderwater) {this->turnsUnderwater = turnsUnderwater;}
    void Submarine::setTorpedoesAmount(int torpedoesAmount) {this->torpedoesAmount = torpedoesAmount;}