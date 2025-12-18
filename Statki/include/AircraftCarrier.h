#pragma once
#include "Ship.h"

class AircraftCarrier : public Ship
{
    protected:
        int aircraftAmount;
        int initialAircraftAmount;
    public:
    AircraftCarrier(std::array<int, 2> position, shipSize size, int fuelAmount, int detectionRange, float health, bool isAlive = true, 
        int aircraftAmount=0);

    bool canAirStrike() const override;

    //gettery
    int getAircraftAmount() const;
    char getSymbol() const override;
    //settery
    void setAircraftAmount(int aircraftAmount);
};