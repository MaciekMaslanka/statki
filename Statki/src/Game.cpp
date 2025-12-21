#include "../include/Game.h"
#include <iostream>

using namespace std;

void Game::switchTurns()
{
    player1->setIsHisTurn(!player1->getIsHisTurn());
    player2->setIsHisTurn(!player2->getIsHisTurn());
}

void Game::placePlayerShips(const int shipsAmount[4], Player* player, int mapSize, int xMax, int xMin)
{
    vector<array<int, 2>> occupiedSpots;
    shipType types[4] = {submarine, aCarrier, destroyer, cruiser};
    for (int t=0; t<4; t++)
    {
        for (int i = 0; i < shipsAmount[t]; i++)
        {
            array<int, 2> pos;
            do
            {
                pos = {
                    (rand() % (xMax - xMin)) + xMin, //x
                    rand() % mapSize //y
                };
            } 
            while (isOccupied(pos, occupiedSpots));
            occupiedSpots.push_back(pos);
            player->addShip(createShip(types[t], pos));
        }
    }
    board->placeFleet(player->getFleet());
}

bool Game::isOccupied(array<int, 2> pos, const vector<array<int, 2>>& occupiedPositions)
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
        
Game::Game() {}

Ship* Game::createShip(shipType type, array<int, 2> position)
{
    switch (type)
    {
        case submarine:
            return new Submarine(position, small, 50, 5, 100, true, 10);
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

//rozgrywka
void Game::beginGame()
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
    placePlayerShips(shipsAmount, player1, mapSize, 10, 0);
    placePlayerShips(shipsAmount, player2, mapSize, mapSize, mapSize-10);

    //ustawienie na planszy
    board->placeFleet(player1->getFleet());
    board->placeFleet(player2->getFleet());
    
    player1->setIsHisTurn(true);
    gameLoop();
}
void Game::gameLoop()
{
    board->display(player1->getFleet());
    board->display(player2->getFleet());
}