#include <iostream>
#include <array>
#include <vector>
#include "platform.h"
using namespace std;

//kolory ansi
const string RESET = "\033[0m";

const string DARK_BLUE_WATER = "\033[34m?\033[0m";
const string LIGHT_BLUE_WATER = "\033[94m~\033[0m";

const string ENEMY_SHIP = "\033[31mE\033[0m";          // czerwony wróg (widoczny!)
const string PLAYER_SHIP = "\033[32mS\033[0m";         // zielony swój statek
const string HIT = "\033[91mX\033[0m";

class Ship
{
    protected:
        array<int, 3> position;
        int length;
        int fuelAmount;
        int detectionRange;
        float health;
        bool isAlive;
        
        int calculateDistance(array<int, 3> from, array<int, 3> to)
        {
            int distance = abs(to[0] - from[0]) + abs(to[1] - from[1]);
            return distance;
        }
    public:
        Ship(array<int, 3> position, int length, int fuelAmount, int detectionRange, float health, bool isAlive = true)
        :position(position), length(length), fuelAmount(fuelAmount), detectionRange(detectionRange), health(health), isAlive(isAlive)
        {}
        virtual bool shootAttack(Ship& target) {return false;}
        virtual bool torpedoAttack(Ship& target) {return false;}
        virtual bool airStrike(Ship& target) {return false;}

        virtual bool canShoot() {return false;}
        virtual bool canTorpedoAttack() {return false;}
        virtual bool canAirStrike() {return false;}

        void move(array<int, 3> targetPosition)
        {
            int distance = calculateDistance(position, targetPosition);
            if (distance <= fuelAmount)
            {
                position = targetPosition;
                fuelAmount -= distance;
            }
            else
            {
                cout<<"Cel jest za daleko"<<endl;
            }
        }

        virtual void takeDamage(float amount)
        {
            health -= amount;
            if (health <= 0)
            {
                sink();
            }
        }

        virtual void sink()
        {
            isAlive = false;
        }

        //gettery
        std::array<int, 3> getPosition() const {return position;}
        int getLength() const {return length;}
        int getFuelAmount() const {return fuelAmount;}
        float getHealth() const {return health;}
        bool getIsAlive() const {return isAlive;}
        int getDetectionRange() const {return detectionRange;}
        //settery
        void setPosition(std::array<int,3 >targetPosition) {position = targetPosition;}
        void setFuelAmount(int targetFuelAmount) {fuelAmount = targetFuelAmount;}

        ~Ship() = default;
};

class Submarine : public Ship
{
    protected:
        bool isUnderwater;
        int turnsUnderWater;
        int torpedoesAmount;
    public:
    Submarine(std::array<int, 3> position, int length, int fuelAmount, int detectionRange, float health, bool isAlive=true, int torpedoesAmount=0)
    :Ship(position, length, fuelAmount, detectionRange, health, isAlive), torpedoesAmount(torpedoesAmount), isUnderwater(false), turnsUnderWater(0)
    {}
    ~Submarine() = default;

    bool canTorpedoAttack() override {return true;}
    
    //gettery
    bool getIsUnderwater() const {return isUnderwater;}
    int getTurnsUnderWater() const {return turnsUnderWater;}
    int getTorpedoesAmount() const {return torpedoesAmount;}
    //settery
    void setIsUnderwater(bool isUnderwater) {this->isUnderwater = isUnderwater;}
    void setTurnsUnderwater(int turnsUnderwater) {this->turnsUnderWater = turnsUnderWater;}
    void setTorpedoesAmount(int torpedoesAmount) {this->torpedoesAmount = torpedoesAmount;}
};

class AircraftCarrier : public Ship
{
    protected:
        int aircraftAmount;
    public:
    AircraftCarrier(array<int, 3> position, int length, int fuelAmount, int detectionRange, float health, bool isAlive = true, int aircraftAmount=0)
    :Ship(position, length, fuelAmount, detectionRange, health, isAlive), aircraftAmount(aircraftAmount)
    {}

    bool canAirStrike() override {return true;}

    //gettery
    int getAircraftAmount() const {return aircraftAmount;}
    //settery
    void setAircraftAmount(int aircraftAmount) {this->aircraftAmount = aircraftAmount;}
};

class Destroyer : public Ship
{
    protected:
        int torpedoesAmount;
    public:
    Destroyer(array<int, 3> position, int length, int fuelAmount, int detectionRange, float health, bool isAlive = true, int torpedoesAmount=0)
    :Ship(position, length, fuelAmount, detectionRange, health, isAlive), torpedoesAmount(torpedoesAmount)
    {}
    bool canShoot() override {return true;}
    bool canTorpedoAttack() override {return true;}
};

class Board
{
    private:
        static const int SIZE = 20;
        char grid[SIZE][SIZE];

        bool isVisible(int x, int y, vector<Ship*>& fleet) const
        {
            for (auto ship : fleet)
            {
                if(ship->getIsAlive() == false) continue;

                array<int, 3> pos = ship->getPosition();
                int shipX = pos[0], shipY = pos[1];
                int dist = abs(shipX - x) + abs(shipY - y);

                if (dist <= ship->getDetectionRange())
                {
                    return true;
                }

            }
            return false;
        }
    public:
        Board()
        {
            for(int i=0; i<SIZE; i++)
            {
                for(int j=0; j<SIZE; j++)
                {
                    grid[i][j] = '.';
                }
            }
        }
        void display(vector<Ship*>& fleet) const
        {
            cout<<"M A P A\n";
            for(int y=0; y<SIZE; y++)
            {
                for(int x=0; x<SIZE; x++)
                {
                    bool visible = isVisible(x, y, fleet);

                    if(visible)
                    {
                        cout<<LIGHT_BLUE_WATER<<" ";
                    }
                    else
                    {
                        cout<<DARK_BLUE_WATER<<" ";
                    }
                }
                cout<<"\n";
            }
            cout<<RESET;
        }
};
class Player
{

};
int main()
{
    Board b1;
    Ship* ubot = new Submarine({10, 10, 0}, 1, 50, 5, 100);
    vector<Ship*> fleet1;
    fleet1.push_back(ubot);
    b1.display(fleet1);
    return 0;
}