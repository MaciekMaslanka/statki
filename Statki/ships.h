#pragma once
#include <array>
#include <vector>

class Ship
{
    protected:
        std::array<int, 3> position;
        int length;
        int fuelAmount;
        int detectionRange;
        float health;
        bool isAlive;
        
    public:
        Ship(std::array<int, 3> position, int length, int fuelAmount, int detectionRange, float health);
        ~Ship() = default;
        virtual void shoot();
        virtual void move();
        virtual void takeDamage(float amount);

        //gettery
        std::array<int, 3> getPosition() const;
        int getLength() const;
        int getFuelAmount() const;
        float getHealth() const;
        bool getIsAlive() const;
        //settery
        void setPosition();
        void setLength();
        void setFuelAmount();
};