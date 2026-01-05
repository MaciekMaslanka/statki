#pragma once
#include "Ship.h"

class AircraftCarrier : public Ship
{
    protected:
        int aircraftAmount;
        int initialAircraftAmount;
        int airStrikeDamage;
    public:
    AircraftCarrier(std::array<int, 2> position, shipType type, int fuelAmount, int detectionRange, float health, bool isAlive = true, 
        int aircraftAmount=0, int airStrikeDamage = 5);

    bool canAirStrike() const override;
    bool tryAirStrike(Ship* target) override;

    //gettery
    int getAircraftAmount() const override;
    int getInitialAircraftAmount() const override;
    char getSymbol() const override;
    //settery
    void setAircraftAmount(int aircraftAmount);
};