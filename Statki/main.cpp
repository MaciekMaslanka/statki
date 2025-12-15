#include <iostream>
#include <array>
#include <vector>
#include <cmath>
#include "platform.h"
using namespace std;

//kolory ansi
const string RESET = "\033[0m";

const string DARK_BLUE_WATER = "\033[90m~\033[0m";
const string LIGHT_BLUE_WATER = "\033[94m~\033[0m";

const string ENEMY_SHIP = "\033[31m";
const string SELECTED_ENEMY_SHIP = "\033[91m";
const string PLAYER_SHIP = "\033[92m";
const string UNDERWATER_PLAYER_SHIP = "\033[32m";
const string SELECTED_PLAYER_SHIP = "\033[96m";
const string HIT = "\033[93m🔥\033[0m";

enum shipSize{small='s', medium='m', large='l'};
class Ship
{
    protected:
        array<int, 2> position;
        char size;
        int fuelAmount;
        int detectionRange;
        float health;
        bool isAlive;
        
        int calculateDistance(array<int, 2> from, array<int, 2> to)
        {
            int distance = abs(to[0] - from[0]) + abs(to[1] - from[1]);
            return distance;
        }
    public:
        Ship(array<int, 2> position, char size, int fuelAmount, int detectionRange, float health, bool isAlive = true)
        :position(position), size(size), fuelAmount(fuelAmount), detectionRange(detectionRange), health(health), isAlive(isAlive)
        {}
        virtual bool shootAttack(Ship& target) {return false;}
        virtual bool torpedoAttack(Ship& target) {return false;}
        virtual bool airStrike(Ship& target) {return false;}

        virtual bool canShoot() {return false;}
        virtual bool canTorpedoAttack() {return false;}
        virtual bool canAirStrike() {return false;}

        void move(array<int, 2> targetPosition)
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
        std::array<int, 2> getPosition() const {return position;}
        int getLength() const {return size;}
        int getFuelAmount() const {return fuelAmount;}
        float getHealth() const {return health;}
        bool getIsAlive() const {return isAlive;}
        int getDetectionRange() const {return detectionRange;}
        virtual int getType() const {return 0;}
        //settery
        void setPosition(std::array<int, 2>targetPosition) {position = targetPosition;}
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
    Submarine(std::array<int, 2> position, char size, int fuelAmount, int detectionRange, float health, bool isAlive=true, int torpedoesAmount=0)
    :Ship(position, size, fuelAmount, detectionRange, health, isAlive), torpedoesAmount(torpedoesAmount), isUnderwater(false), turnsUnderWater(0)
    {}
    ~Submarine() = default;

    bool canTorpedoAttack() override {return true;}
    
    //gettery
    bool getIsUnderwater() const {return isUnderwater;}
    int getTurnsUnderWater() const {return turnsUnderWater;}
    int getTorpedoesAmount() const {return torpedoesAmount;}
    int getType() const {return 1;}
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
    AircraftCarrier(array<int, 2> position, char size, int fuelAmount, int detectionRange, float health, bool isAlive = true, int aircraftAmount=0)
    :Ship(position, size, fuelAmount, detectionRange, health, isAlive), aircraftAmount(aircraftAmount)
    {}

    bool canAirStrike() override {return true;}

    //gettery
    int getAircraftAmount() const {return aircraftAmount;}
    int getType() const {return 2;}
    //settery
    void setAircraftAmount(int aircraftAmount) {this->aircraftAmount = aircraftAmount;}
};

class Destroyer : public Ship
{
    protected:
        int torpedoesAmount;
    public:
    Destroyer(array<int, 2> position, char size, int fuelAmount, int detectionRange, float health, bool isAlive = true, int torpedoesAmount=0)
    :Ship(position, size, fuelAmount, detectionRange, health, isAlive), torpedoesAmount(torpedoesAmount)
    {}
    bool canShoot() override {return true;}
    bool canTorpedoAttack() override {return true;}

    int getType() const {return 3;}
};

class Cruiser : public Ship
{

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

                array<int, 2> pos = ship->getPosition();
                int shipX = pos[0], shipY = pos[1];
                float dx = shipX - x;
                float dy = shipY - y;
                float dist = ceil(sqrt(dx*dx + dy*dy));
                if (dist <= ship->getDetectionRange())
                {
                    return true;
                }

            }
            return false;
        }
        void placePlayerShip(Ship* ship)
        {
            array<int, 2> pos = ship->getPosition();
            /*typy statków:
                0- podstawowy
                1-submarine
                2-aircraft carrier
                3-destroyer
            */
           int shipType = ship->getType();
           switch (shipType)
           {
            case 1: //submarine
                grid[pos[1]][pos[0]] = 'S';
                break;
            case 2: //aircraft carrier
                grid[pos[1]][pos[0]] = 'A';
                break;
            case 3: //destroyer
                grid[pos[1]][pos[0]] = 'D';
                break;
           default:
            break;
           }
        }
        void placeEnemyShip(Ship* ship)
        {
            array<int, 2> pos = ship->getPosition();
            if (ship->getType() == 1 && static_cast<Submarine*>(ship)->getIsUnderwater() == true)
            {
                return;
            }
            grid[pos[1]][pos[0]] = 'E';
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
        void display(vector<Ship*>& playerFleet, vector<Ship*> enemyFleet)
        {
            //ustawienie statków gracza na planszy
            for(Ship* ship : playerFleet)
            {
                placePlayerShip(ship);
            }
            //ustawienie statków wroga na planszy
            for(Ship* ship : enemyFleet)
            {
                placeEnemyShip(ship);
            }
            //wyswietlenie
            //zrobić tak żeby tablica miała odwołania do statków a nie znaki
            for(int y=0; y<SIZE; y++)
            {
                for(int x=0; x<SIZE; x++)
                {
                    bool visible = isVisible(x, y, playerFleet);
                    if(grid[y][x] == 'S') //submarine
                    {
                        cout<<PLAYER_SHIP<<"S "<<RESET;
                    }
                    else if(grid[y][x] == 'A') //aircraft carrier
                    {
                        cout<<PLAYER_SHIP<<"A "<<RESET;
                    }
                    else if(grid[y][x] == 'D') //destroyer
                    {
                        cout<<PLAYER_SHIP<<"D "<<RESET;
                    }
                    else if(grid[y][x] == 'E') //enemy ship
                    {
                        if(visible)
                        {
                            cout<<ENEMY_SHIP<<"E "<<RESET;
                        }
                        else
                        {
                            cout<<DARK_BLUE_WATER<<" ";
                        }
                    }
                    else //woda
                    {
                        if(isVisible(x, y, playerFleet))
                        {
                            cout<<LIGHT_BLUE_WATER<<" ";
                        }
                        else
                        {
                            cout<<DARK_BLUE_WATER<<" ";
                        }
                    }
                }
                cout<<"\n";
            }
            cout<<RESET;
        }
};
class Player
{
    protected:
        vector<Ship*> fleet;
        int movePoints;
    public:
        Player(vector<Ship*> fleet, int movePoints)
        :fleet(fleet), movePoints(movePoints)
        {}
        ~Player() = default;
        void addShip(Ship* ship)
        {
            fleet.push_back(ship);
        }
        //gettery
        vector<Ship*> getFleet() const {return fleet;}
        int getMovePoints() const {return movePoints;}
        //settery
        void setMovePoints(int movePoints) {this->movePoints = movePoints;}
};
int main()
{
    Board b1;
    Ship* ubot = new Submarine({10, 5}, medium, 50, 5, 100);
    Ship* ubot2 = new Submarine({9, 5}, medium, 50, 5, 100);
    static_cast<Submarine*>(ubot2)->setIsUnderwater(false);
    Ship* carrier = new AircraftCarrier({7, 10}, large, 50, 7, 100);
    Ship* destroyer = new Destroyer({5, 5}, medium, 50, 6, 100);
    vector<Ship*> fleet1;
    vector<Ship*> fleet2;
    fleet1.push_back(ubot);
    fleet1.push_back(carrier);
    fleet2.push_back(destroyer);
    fleet2.push_back(ubot2);
    b1.display(fleet1, fleet2);
    return 0;
}