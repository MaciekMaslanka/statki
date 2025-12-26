#pragma once
#include <array>
#include <iostream>

enum shipSize{small, medium, large};
enum shipType{submarine, aCarrier, destroyer, cruiser};

class Ship
{
    protected:
        std::array<int, 2> position;
        shipSize size;
        shipType type;
        int fuelAmount;
        int initialFuelAmount;
        int detectionRange;
        float health;
        float initialHealth;
        bool isAlive;
        
        int calculateDistance(std::array<int, 2> from, std::array<int, 2> to);

    public:
        Ship(std::array<int, 2> position, shipSize size, shipType type, int fuelAmount, int detectionRange, float health, bool isAlive = true);

        virtual bool shootAttack(Ship& target);
        virtual bool torpedoAttack(Ship& target);
        virtual bool airStrike(Ship& target);

        virtual bool canShoot() const;
        virtual bool canTorpedoAttack() const;
        virtual bool canAirStrike() const;

        virtual bool isStealth() const;
        virtual bool toogleDive();

        void move(std::array<int, 2> targetPosition);

        virtual void takeDamage(float amount);

        virtual void sink();

        //gettery
        std::array<int, 2> getPosition() const;
        shipSize getSize() const;
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
