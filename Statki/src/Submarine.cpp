#include "../include/Submarine.h"


    Submarine::Submarine(
        std::array<int, 2> position, shipType type, int fuelAmount, int detectionRange, float health, bool isAlive, 
        int torpedoesAmount)
    :Ship(position, type, fuelAmount, detectionRange, health, isAlive), torpedoesAmount(torpedoesAmount), 
    isUnderwater(false), turnsUnderwater(0), initialTorpedoesAmount(torpedoesAmount)
    {}

    bool Submarine::canTorpedoAttack() const {return true;}
    bool Submarine::isStealth() const {return isUnderwater && isAlive;}
    bool Submarine::canDive() const {return true;}
    bool Submarine::toogleDive()
    {
        if (!isAlive) return false;
        isUnderwater = !isUnderwater;
        if (isUnderwater)
        {
            detectionRange /= 2;
        }
        else
        {
            detectionRange *= 2;
            turnsUnderwater = 0;
        }
        return isUnderwater;
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