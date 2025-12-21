#include "../include/Board.h"
using namespace std;


bool Board::isVisible(int x, int y, const vector<Ship*>& fleet) const
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

Board::Board(int size)
{
    mapSize = size;
    grid.resize(size, vector<Ship*>(size, nullptr));
}

void Board::placeFleet(const vector<Ship*>& fleet)
{
    for(auto ship : fleet)
    {
        array<int, 2> pos = ship->getPosition();
        grid[pos[1]][pos[0]] = ship;
    }
}

void Board::display(const vector<Ship*>& playerFleet)
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
                for(const Ship* ship : playerFleet)
                {
                    if(ship == tile)
                    {
                        isPlayerShip = true;
                        break;
                    }
                }

                if(isPlayerShip)
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
                        if (tile->getIsAlive())
                        {
                            cout<<ENEMY_SHIP<<tile->getSymbol()<<RESET<<" ";
                        }
                        else
                        {
                            cout<<DESTROYED;
                        }
                    }
                    else if (tile->isStealth())
                    {
                        cout<<LIGHT_BLUE_WATER<<" ";
                    }
                    else
                    {
                        cout<<DARK_BLUE_WATER<<" ";
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
}

bool Board::getIsInCursorMode() const {return isInCursorMode;}
void Board::setIsInCursorMode(bool mode) {isInCursorMode = mode;}