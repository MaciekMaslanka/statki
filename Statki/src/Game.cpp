#include "../include/Game.h"
#include <iostream>

using namespace std;

const string SPLITTER = "\n-------------------------------------\n";

const char cursorModeKey = 'c';
const char moveKey = 'm';
const char shootAttackKey = 'o';
const char torpedoAttackKey = 't';
const char airStrikeKey = 'a';
const char cancelKey = 'q';

const int movePointsPerTurn = 5;

void Game::switchTurns()
{
    if (currentPlayer == player1)
    {
        currentPlayer = player2;
    }
    else
    {
        currentPlayer = player1;
    }
    currentPlayer->setMovePoints(movePointsPerTurn);
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

bool Game::isCurrentPlayerShip(Ship* ship) const
{
    const vector<Ship*>& fleet = currentPlayer->getFleet();
    for (Ship* playerShip : fleet)
    {
        if (playerShip == ship)
        {
            return true;
        }
    }
    return false;
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
            return new Submarine(position, small, submarine, 50, 5, 100, true, 10);
        case aCarrier:
            return new AircraftCarrier(position, large, aCarrier, 50, 7, 100);
        case destroyer:
            return new Destroyer(position, medium, destroyer, 50, 6, 100);
        case cruiser:
            return new Cruiser(position, large, cruiser, 50, 6, 100);
        default:
            return nullptr;
    }
}

//rozgrywka
void Game::beginGame()
{
    srand(time(NULL));
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
        if (mapSize < 20 || mapSize > 50)
        {
            cout<<"Nieprawidłowy rozmiar mapy. Spróbuj ponownie.\n";
        }
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
    
    //losowy gracz zaczyna
    if (rand() % 2 == 0)
    {
        currentPlayer = player1;
    }
    else
    {
        currentPlayer = player2;
    }
    gameLoop();
}
void Game::displayGame()
{
    clearScreen();
    cout<<"Tura gracza "<<currentPlayer->getName()<<endl;
    cout<<SPLITTER;

    //mapa
    board->display(currentPlayer->getFleet());
    cout<<SPLITTER;
    
    //statystyki
    bool isInCursorMode = board->getIsInCursorMode();
    bool isInMoveMode = board->getIsInMoveMode();
    Ship* ship = board->getShipUnderCursor();
    if (isInCursorMode && !isInMoveMode)
    {
        cout<<"Statystyki okrętu pod kursorem: \n";
        if (ship != nullptr && isCurrentPlayerShip(ship))
        {
            cout<<"Typ: ";
            switch (ship->getType())
            {
                case submarine:
                    cout<<"Okręt podwodny\n";
                    break;
                case aCarrier:
                    cout<<"Lotniskowiec\n";
                    break;
                case destroyer:
                    cout<<"Niszczyciel\n";
                    break;
                case cruiser:
                    cout<<"Krążownik\n";
                    break;
                default:
                    cout<<"????\n";
                    break;
            }
            float fuelPercent = (ship->getFuelAmount() * 100) / ship->getInitialFuelAmount();
            float healthPercent = (ship->getHealth() * 100) / ship->getInitialHealth();
            cout<<"Paliwo: "<<fuelPercent<<"% ("<<ship->getFuelAmount()<<"/"<<ship->getInitialFuelAmount()<<")\n";
            cout<<"Zdrowie: "<<healthPercent<<"% ("<<ship->getHealth()<<"/"<<ship->getInitialHealth()<<")\n";
            cout<<"Status: "<<(ship->getIsAlive() ? "Aktywny\n" : "Zatopiony\n");
        }
        else
        {
            cout<<"Brak okrętu pod kursorem.\n";
        }
        cout<<"Wciśnij"<<cancelKey<<", aby wyjść z trybu kursora.\n";
    }
    else if (isInMoveMode)
    {
        cout<<"Wskaż docelową pozycję.\n";
        if (!selectedShip) return;

        int moveCost = selectedShip->calculateMoveCost(board->getCursorPosition());
        Ship* underCursor = board->getShipUnderCursor();
        if (moveCost != -1 && underCursor == nullptr)
        {
            cout<<"Statek zużyje "<<moveCost<<" jednostek paliwa aby tu dopłynąć.\n";
        }
        else if (moveCost == -1)
        {
            cout<<"Ta pozycja jest za daleko!\n";
        }
        else
        {
            cout<<"Ta pozycja jest zajęta przez inny statek!\n";
        }
    }
    else
    {
        cout<<"Wciśnij 'c', aby wejść w tryb kursora i zobaczyć statystyki i opcje okrętów.\n";
    }

    cout<<"Pozostałe punkty ruchu: "<<currentPlayer->getMovePoints()<<endl;
    cout<<SPLITTER;

    //reszta wiadomosci
    if (currentPlayer->getMovePoints() > 0)
    {
        if (isInCursorMode && !isInMoveMode)
        {
            if (ship != nullptr && ship->getIsAlive())
            {
                cout<<moveKey<<"- przesuń okręt\n";
                if (ship->canShoot())
                {
                    cout<<shootAttackKey<<"- atak ostrzałem\n";
                }
                if (ship->canTorpedoAttack())
                {
                    cout<<torpedoAttackKey<<"- atak torpedami\n";
                }
                if (ship->canAirStrike())
                {
                    cout<<airStrikeKey<<"- atak lotniczy\n";
                }
            }
            else if (ship != nullptr && !ship->getIsAlive())
            {
                cout<<"Ten okręt jest zatopiony. Brak dostępnych opcji.\n";
            }
            else
            {
                cout<<"Brak okrętu pod kursorem.\n";
            }
            cout<<cancelKey<<"- wyjdź z trybu kursora\n";        
        }
        else if (isInMoveMode)
        {
            cout<<moveKey<<"- potwierdź ruch\n";
            cout<<cancelKey<<"- anuluj\n";
        }
    }
    else
    {
        cout<<"Nie posiadasz punktów ruchu!\n";
    }

}
void Game::gameLoop()
{
    while (true)
    {
        clearScreen();
        displayGame();
        char key = getKey();

        bool canMoveToTile;

        //wlaczenie kursora
        if (key == cursorModeKey && !board->getIsInCursorMode())
        {
            board->toogleCursorMode();
            continue;
        }

        if (key == moveKey)
        {
            //potwierdzenie ruchu
            if (board->getIsInMoveMode())
            {
                Ship* tile = board->getShipUnderCursor();
                if (tile == nullptr)
                {
                    array<int, 2> targetPos = board->getCursorPosition();
                    int moveCost = selectedShip->calculateMoveCost(targetPos);
                    if (moveCost != -1 && currentPlayer->getMovePoints() > 0)
                    {
                        board->updateShipPosition(selectedShip, targetPos);
                        board->toogleMoveMode(nullptr);
                        currentPlayer->setMovePoints(currentPlayer->getMovePoints() - 1);
                        continue;
                    }
                }
            }

            //wlaczenie trybu ruchu
            if (board->getIsInCursorMode() && currentPlayer->getMovePoints() > 0)
            {
                Ship* ship = board->getShipUnderCursor();
                if (ship != nullptr && isCurrentPlayerShip(ship) && ship->getIsAlive())
                {
                    selectedShip = ship;
                    board->toogleMoveMode(selectedShip);
                }
                continue;
            }
        }

        //wylaczanie trybow
        if (key == cancelKey)
        {
            if (board->getIsInMoveMode())
            {
                board->toogleMoveMode(nullptr);
                continue;
            }
            else if (board->getIsInCursorMode())
            {
                board->toogleCursorMode();
                continue;
            }
        }

        //ruch kursorem
        if (board->getIsInCursorMode())
        {
            array<int, 2> offset = {0, 0};
            switch (key)
            {
                case 'w':
                    offset = {0, -1};
                    break;
                case 's':
                    offset = {0, 1};
                    break;
                case 'a':
                    offset = {-1, 0};
                    break;
                case 'd':
                    offset = {1, 0};
                    break;
                default:
                    break;
            }
            board->moveCursor(offset);
            continue;
        }
    }
}