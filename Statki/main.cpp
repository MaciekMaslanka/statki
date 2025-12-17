#include <iostream>
#include <array>
#include <vector>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include <algorithm>
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
const string DESTROYED = "\033[93m🔥\033[0m";

enum shipSize{small='s', medium='m', large='l'};
enum shipType{submarine, aCarrier, destroyer, cruiser};
class Ship
{
    protected:
        array<int, 2> position;
        char size;
        int fuelAmount;
        int initialFuelAmount;
        int detectionRange;
        float health;
        float initialHealth;
        bool isAlive;
        
        int calculateDistance(array<int, 2> from, array<int, 2> to)
        {
            int distance = abs(to[0] - from[0]) + abs(to[1] - from[1]);
            return distance;
        }
    public:
        Ship(array<int, 2> position, char size, int fuelAmount, int detectionRange, float health, bool isAlive = true)
        :position(position), size(size), fuelAmount(fuelAmount), detectionRange(detectionRange), 
        health(health), isAlive(isAlive), initialFuelAmount(fuelAmount), initialHealth(health)
        {}
        virtual bool shootAttack(Ship& target) {return false;}
        virtual bool torpedoAttack(Ship& target) {return false;}
        virtual bool airStrike(Ship& target) {return false;}

        virtual bool canShoot() {return false;}
        virtual bool canTorpedoAttack() {return false;}
        virtual bool canAirStrike() {return false;}

        virtual bool isStealth() const {return false;}
        virtual bool toogleDive() {return false;}

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
        char getSize() const {return size;}
        int getFuelAmount() const {return fuelAmount;}
        float getHealth() const {return health;}
        bool getIsAlive() const {return isAlive;}
        int getDetectionRange() const {return detectionRange;}
        virtual char getSymbol() const {return '?';}
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
        int initialTorpedoesAmount;
    public:
    Submarine(std::array<int, 2> position, char size, int fuelAmount, int detectionRange, float health, bool isAlive=true, int torpedoesAmount=0)
    :Ship(position, size, fuelAmount, detectionRange, health, isAlive), torpedoesAmount(torpedoesAmount), 
    isUnderwater(false), turnsUnderWater(0), initialTorpedoesAmount(torpedoesAmount)
    {}
    ~Submarine() = default;

    bool canTorpedoAttack() override {return true;}
    bool isStealth() const override
    {
        return isUnderwater && isAlive;
    }
    bool toogleDive() override
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
            turnsUnderWater = 0;
        }
        return isUnderwater;
    }
    
    //gettery
    bool getIsUnderwater() const {return isUnderwater;}
    int getTurnsUnderWater() const {return turnsUnderWater;}
    int getTorpedoesAmount() const {return torpedoesAmount;}
    char getSymbol() const override {return 'S';}
    //settery
    void setIsUnderwater(bool isUnderwater) {this->isUnderwater = isUnderwater;}
    void setTurnsUnderwater(int turnsUnderwater) {this->turnsUnderWater = turnsUnderWater;}
    void setTorpedoesAmount(int torpedoesAmount) {this->torpedoesAmount = torpedoesAmount;}
};

class AircraftCarrier : public Ship
{
    protected:
        int aircraftAmount;
        int initialAircraftAmount;
    public:
    AircraftCarrier(array<int, 2> position, char size, int fuelAmount, int detectionRange, float health, bool isAlive = true, int aircraftAmount=0)
    :Ship(position, size, fuelAmount, detectionRange, health, isAlive), aircraftAmount(aircraftAmount), initialAircraftAmount(aircraftAmount)
    {}

    bool canAirStrike() override {return true;}

    //gettery
    int getAircraftAmount() const {return aircraftAmount;}
    char getSymbol() const override {return 'A';}
    //settery
    void setAircraftAmount(int aircraftAmount) {this->aircraftAmount = aircraftAmount;}
};

class Destroyer : public Ship
{
    protected:
        int torpedoesAmount;
        int initialTorpedoesAmount;
    public:
    Destroyer(array<int, 2> position, char size, int fuelAmount, int detectionRange, float health, bool isAlive = true, int torpedoesAmount=0)
    :Ship(position, size, fuelAmount, detectionRange, health, isAlive), torpedoesAmount(torpedoesAmount), initialTorpedoesAmount(torpedoesAmount)
    {}
    bool canShoot() override {return true;}
    bool canTorpedoAttack() override {return true;}

    char getSymbol() const override {return 'D';}
};

class Cruiser : public Ship
{
    public:
    Cruiser(array<int, 2> position, char size, int fuelAmount, int detectionRange, float health, bool isAlive = true)
    :Ship(position, size, fuelAmount, detectionRange, health, isAlive)
    {
    }
    char getSymbol() const override {return 'C';}
};

class Board
{
    private:
        int mapSize;
        vector<vector<Ship*>> grid;

        bool isVisible(int x, int y, vector<Ship*>& fleet) const
        {
            for (const Ship* ship : fleet)
            {
                if(!ship->getIsAlive()) continue;

                array<int, 2> shipPos = ship->getPosition();
                int dist = abs(shipPos[0] - x) + abs(shipPos[1] - y);
                if (dist <= ship->getDetectionRange())
                {
                    return true;
                }
            }
            return false;
        }
    public:
        Board(int size)
        {
            mapSize = size;
            grid.resize(size, vector<Ship*>(size, nullptr));
        }
        void placeFleet(const vector<Ship*>& fleet)
        {
            for(auto ship : fleet)
            {
                array<int, 2> pos = ship->getPosition();
                grid[pos[1]][pos[0]] = ship;
            }
        }
        void display(vector<Ship*>& playerFleet)
        {
            //wyswietlanie planszy
            for(int y=0; y<mapSize; y++)
            {
                for(int x=0; x<mapSize; x++)
                {
                    Ship* tile = grid[y][x];
                    if(tile != nullptr) //jezeli jest jakis statek
                    {
                        bool isPlayerShip = false;
                        for(auto ship : playerFleet)
                        {
                            if(ship == tile)
                            {
                                isPlayerShip = true;
                                break;
                            }
                        }

                        if(isPlayerShip) //jezeli jest gracza
                        {
                            if (tile->isStealth())
                            {
                                cout<<UNDERWATER_PLAYER_SHIP<<tile->getSymbol()<<RESET<<" ";
                            }
                            else if (tile->getIsAlive() == false)
                            {
                                cout<<DESTROYED;
                            }
                            else
                            {
                                cout<<PLAYER_SHIP<<tile->getSymbol()<<RESET<<" ";
                            }
                        }
                        else //jezeli jest wroga
                        {
                            if (isVisible(x, y, playerFleet) && !tile->isStealth())
                            {
                                if (tile->getIsAlive() == false)
                                {
                                    cout<<DESTROYED;
                                }
                                else
                                {
                                    cout<<ENEMY_SHIP<<tile->getSymbol()<<RESET<<" ";
                                }
                                
                            }
                            else
                            {
                                cout<<LIGHT_BLUE_WATER<<" ";
                            }
                        }
                    }
                    else //jezeli nie ma statku
                    {
                        if (isVisible(x, y, playerFleet))
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
            cout<<"\n-------------------------------------\n";
            //wyswietlanie statystyk
        }
};
class Player
{
    protected:
        vector<Ship*> fleet;
        string name;
        int movePoints;
        bool isHisTurn;
    public:
        Player(vector<Ship*> fleet, int movePoints, string name="")
        :fleet(fleet), movePoints(movePoints), isHisTurn(false), name(name)
        {}
        ~Player() = default;
        void addShip(Ship* ship)
        {
            fleet.push_back(ship);
        }
        //gettery
        vector<Ship*> getFleet() const {return fleet;}
        int getMovePoints() const {return movePoints;}
        bool getIsHisTurn() const {return isHisTurn;}
        //settery
        void setMovePoints(int movePoints) {this->movePoints = movePoints;}
        void setIsHisTurn(bool isHisTurn) {this->isHisTurn = isHisTurn;}
};
class Game
{
    private:
        Board* board = nullptr;
        Player* player1 = nullptr;
        Player* player2 = nullptr;

        void switchTurns()
        {
            player1->setIsHisTurn(!player1->getIsHisTurn());
            player2->setIsHisTurn(!player2->getIsHisTurn());
        }

        void placeShips(int shipsAmount[4], Player* player1, Player* player2, int mapSize)
        {
            vector<array<int, 2>> occupiedPositions;
            //gracz 1
            for (int i=0; i<shipsAmount[0]; i++) //submariny
            {
                srand(time(0));
                array<int, 2> position;
                do
                {
                    srand(time(0));
                    position = {rand() % 10, rand() % mapSize};
                } 
                while(isOccupied(position, occupiedPositions));
                occupiedPositions.push_back(position);
                Ship* newShip = createShip(submarine, position);
                player1->addShip(newShip);
            }
            for (int i=0; i<shipsAmount[1]; i++) //lotniskowce
            {
                array<int, 2> position;
                do
                {
                    srand(time(0));
                    position = {rand() % 10, rand() % mapSize};
                } 
                while(isOccupied(position, occupiedPositions));
                occupiedPositions.push_back(position);
                Ship* newShip = createShip(aCarrier, position);
                player1->addShip(newShip);
            }
            for (int i=0; i<shipsAmount[2]; i++) //niszczyciele
            {
                array<int, 2> position;
                do
                {
                    srand(time(0));
                    position = {rand() % 10, rand() % mapSize};
                } 
                while(isOccupied(position, occupiedPositions));
                occupiedPositions.push_back(position);
                Ship* newShip = createShip(destroyer, position);
                player1->addShip(newShip);
            }
            for (int i=0; i<shipsAmount[3]; i++) //krążowniki
            {
                array<int, 2> position;
                do
                {
                    srand(time(0));
                    position = {rand() % 10, rand() % mapSize};
                } 
                while(isOccupied(position, occupiedPositions));
                occupiedPositions.push_back(position);
                Ship* newShip = createShip(cruiser, position);
                player1->addShip(newShip);
            }
            occupiedPositions.clear();

            //gracz 2
            for (int i=0; i<shipsAmount[0]; i++) //submariny
            {
                array<int, 2> position;
                do
                {
                    srand(time(0));
                    position = {rand() % 10 + (mapSize - 10), rand() % mapSize};
                } 
                while(isOccupied(position, occupiedPositions));
                occupiedPositions.push_back(position);
                Ship* newShip = createShip(submarine, position);
                player2->addShip(newShip);
            }
            for (int i=0; i<shipsAmount[1]; i++) //lotniskowce
            {
                array<int, 2> position;
                do
                {
                    srand(time(0));
                    position = {rand() % 10 + (mapSize - 10), rand() % mapSize};
                } 
                while(isOccupied(position, occupiedPositions));
                occupiedPositions.push_back(position);
                Ship* newShip = createShip(aCarrier, position);
                player2->addShip(newShip);
            }
            for (int i=0; i<shipsAmount[2]; i++) //niszczyciele
            {
                array<int, 2> position;
                do
                {
                    srand(time(0));
                    position = {rand() % 10 + (mapSize - 10), rand() % mapSize};
                } 
                while(isOccupied(position, occupiedPositions));
                occupiedPositions.push_back(position);
                Ship* newShip = createShip(destroyer, position);
                player2->addShip(newShip);
            }
            for (int i=0; i<shipsAmount[3]; i++) //krążowniki
            {
                array<int, 2> position;
                do
                {
                    srand(time(0));
                    position = {rand() % 10 + (mapSize - 10), rand() % mapSize};
                } 
                while(isOccupied(position, occupiedPositions));
                occupiedPositions.push_back(position);
                Ship* newShip = createShip(cruiser, position);
                player2->addShip(newShip);
            }
        }

        bool isOccupied(array<int, 2> pos, vector<array<int, 2>>& occupiedPositions)
        {
            for (array<int, 2> occupiedPos : occupiedPositions)
            {
                if (occupiedPos == pos)
                {
                    return true;
                }
            }
            return false;
        }
        
    public:
        Game() {}
        Ship* createShip(shipType type, array<int, 2> position)
        {
            switch (type)
            {
                case submarine:
                    return new Submarine(position, medium, 50, 5, 100);
                case aCarrier:
                    return new AircraftCarrier(position, large, 50, 7, 100);
                case destroyer:
                    return new Destroyer(position, medium, 50, 6, 100);
                case cruiser:
                    return new Cruiser(position, large, 50, 6, 100);
                default:
                    return nullptr;
            }
        }
        ~Game() = default;

        //rozgrywka
        void beginGame()
        {
            string name1, name2;
            int shipsAmount[4] = {0, 0, 0, 0}; //podwodne, lotniskowce, niszczyciele, krążowniki
            int mapSize;

            //nicki graczy
            cout<<"Podaj nick gracza 1: ";
            getline(cin, name1);
            cout<<"Podaj nick gracza 2: ";
            getline(cin, name2);

            //rozmiar mapy
            do
            {
                cout<<"Podaj rozmiar mapy (min 20, max 50): ";
                cin>>mapSize;
            } while (mapSize < 20 || mapSize > 50);
            board = new Board(mapSize);

            //flota
            cout<<"Podaj ilość okrętów podwodnych: ";
            cin>>shipsAmount[0];
            cout<<"Podaj ilość lotniskowców: ";
            cin>>shipsAmount[1];
            cout<<"Podaj ilość niszczycieli: ";
            cin>>shipsAmount[2];
            cout<<"Podaj ilość krążowników: ";
            cin>>shipsAmount[3];

            vector<Ship*> fleet1;
            vector<Ship*> fleet2;
            player1 = new Player(fleet1, 5, name1);
            player2 = new Player(fleet2, 5, name2);
            //gracz 1 zaczyna po lewej a 2 po prawej
            placeShips(shipsAmount, player1, player2, mapSize);

            //ustawienie na planszy
            board->placeFleet(player1->getFleet());
            board->placeFleet(player2->getFleet());
            
            player1->setIsHisTurn(true);
            gameLoop();
        }
        void gameLoop()
        {

        }
};
int main()
{
    // Game game(nullptr, nullptr);
    // Ship* ubot = game.createShip(submarine, {10, 5});
    // Ship* ubot2 = game.createShip(submarine, {9, 5});
    // Ship* carrier = game.createShip(aCarrier, {7, 10});
    // Ship* dest = game.createShip(destroyer, {5, 5});
    // vector<Ship*> fleet1;
    // vector<Ship*> fleet2;
    // fleet1.push_back(ubot);
    // fleet1.push_back(carrier);
    // fleet2.push_back(dest);
    // fleet2.push_back(ubot2);
    // b1.placeFleet(fleet1);
    // b1.placeFleet(fleet2);
    // b1.display(fleet1);
    // sleepMs(3000);
    // clearScreen();
    // ubot->toogleDive();
    // b1.display(fleet1);
    // sleepMs(3000);
    // clearScreen();
    // b1.display(fleet2);
    return 0;
}