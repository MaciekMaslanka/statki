#pragma once
#include <array>
#include <iostream>

enum shipType{submarine, aCarrier, destroyer, cruiser};

class Ship
{
    protected:
        std::array<int, 2> position;
        shipType type;
        int fuelAmount;
        int initialFuelAmount;
        int detectionRange;
        float health;
        float initialHealth;
        bool isAlive;
        
        int calculateDistance(std::array<int, 2> from, std::array<int, 2> to);

    public:
        Ship(std::array<int, 2> position, shipType type, int fuelAmount, int detectionRange, float health, bool isAlive = true);

        virtual bool canShoot() const;
        virtual bool canTorpedoAttack() const;
        virtual bool canAirStrike() const;

        virtual bool tryShootAttack(Ship* target);
        virtual bool tryTorpedoAttack(Ship* target);
        virtual bool tryAirStrike(Ship* target);

        virtual int getShootAttackDmg() const;
        virtual int getTorpedoAttackDmg() const;
        virtual int getAirStrikeDmg() const;

        virtual bool isStealth() const;
        virtual bool canDive() const;
        virtual bool toogleDive();

        void takeDamage(float amount);
        virtual void onNewTurn();

        void move(std::array<int, 2> targetPosition);
        int calculateMoveCost(std::array<int, 2> targetPosition);

        //gettery
        std::array<int, 2> getPosition() const;
        shipType getType() const;
        int getFuelAmount() const;
        int getInitialFuelAmount() const;
        float getHealth() const;
        float getInitialHealth() const;
        bool getIsAlive() const;
        int getDetectionRange() const;
        virtual char getSymbol() const;
        //settery
        void setPosition(std::array<int, 2>targetPosition);
        void setFuelAmount(int targetFuelAmount);

        virtual ~Ship() = default;
};