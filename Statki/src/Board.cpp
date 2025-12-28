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
bool Board::isVisible(int x, int y, const Ship* ship) const
{
    if (!ship->getIsAlive()) { return false; }

    array<int, 2> shipPos = ship->getPosition();
    int dist = abs(shipPos[0] - x) + abs(shipPos[1] - y);
    if (dist <= ship->getDetectionRange())
    {
        return true;
    }
    else
    {
        return false;
    }

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

void Board::drawTile(string symbol, const string& color, bool isCursorHere) const
{
    if (isCursorHere)
    {
        cout<<CURSOR;
    }
        
    cout<<color<<symbol<<RESET<<" ";
}

void Board::display(const vector<Ship*>& playerFleet)
{
    //wyswietlanie planszy
    for(int y=0; y<mapSize; y++)
    {
        for(int x=0; x<mapSize; x++)
        {
            Ship* tile = grid[y][x];
            string symbol = "~";
            string color = DARK_BLUE_WATER;
            bool visible  = isVisible(x, y, playerFleet);
            bool isCursorHere = (cursorPosition[0] == x && cursorPosition[1] == y && isInCursorMode);

            if(tile == nullptr)
            {
                if (visible && !isInMoveMode)
                {
                    color = LIGHT_BLUE_WATER;
                }
                else if (isInMoveMode)
                {
                    //sprawdzanie czy moze sie tam przesunac
                    if (isVisible(x, y, selectedShip))
                    {
                        color = YELLOW;
                    }
                    else
                    {
                        color = DARK_BLUE_WATER;
                    }
                }
            }
            else
            {
                bool isPlayerShip = false;
                for (const Ship* playerShip : playerFleet)
                {
                    if (tile == playerShip)
                    {
                        isPlayerShip = true;
                        break;
                    }
                }
            
                if (!tile->getIsAlive())
                {
                    symbol = DESTROYED;
                }
                else if (isPlayerShip)
                {
                    symbol = tile->getSymbol();
                    if (tile->isStealth())
                    {
                        color = UNDERWATER_PLAYER_SHIP;
                    }
                    else
                    {
                        color = PLAYER_SHIP;
                    }
                }
                else if (visible && !tile->isStealth())
                {
                    symbol = tile->getSymbol();
                    color = ENEMY_SHIP;
                }
                else
                {
                    symbol = "~";
                    color = DARK_BLUE_WATER;
                }
            }

            drawTile(symbol, color, isCursorHere);

        }
        cout<<"\n";
    }
}

bool Board::getIsInCursorMode() const {return isInCursorMode;}
void Board::toogleCursorMode() {isInCursorMode = !isInCursorMode;}
array<int, 2> Board::getCursorPosition() const {return cursorPosition;}
Ship* Board::getShipUnderCursor() const
{
    return grid[cursorPosition[1]][cursorPosition[0]];
}
void Board::moveCursor(array<int, 2> offset)
{
    int newX = cursorPosition[0] + offset[0];
    int newY = cursorPosition[1] + offset[1];

    if (newX >= 0 && newX < mapSize)
    {
        cursorPosition[0] = newX;
    }
    if (newY >= 0 && newY < mapSize)
    {
        cursorPosition[1] = newY;
    }
}

void Board::toogleMoveMode(Ship* ship)
{
    if (ship == nullptr)
    {
        isInMoveMode = false;
        selectedShip = nullptr;
    }
    else
    {
        isInMoveMode = !isInMoveMode;
        selectedShip = ship;
    }
}
bool Board::getIsInMoveMode() const {return isInMoveMode;}