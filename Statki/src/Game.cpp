#include "../include/Game.h"
#include <iostream>

using namespace std;

const string SPLITTER = "\n-------------------------------------\n";

//config
const char cursorModeKey = 'c';
const char moveKey = 'm';
const char shootAttackKey = 'j';
const char torpedoAttackKey = 'k';
const char airStrikeKey = 'l';
const char cancelKey = 'q';
const char endTurnKey = 'e';
enum moveKeys{up='w', down='s', left='a', right='d'};

const int movePointsPerTurn = 5;

void Game::switchTurns()
{
    for (Ship* ship : currentPlayer->getFleet())
    {
        if (ship != nullptr)
        {
            ship->onNewTurn();
        }
    }

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
            return new Submarine(position, submarine, 50, 5, 100, true, 10, 5);
        case aCarrier:
            return new AircraftCarrier(position, aCarrier, 50, 7, 100, true, 100, 5);
        case destroyer:
            return new Destroyer(position, destroyer, 50, 6, 4, true);
        case cruiser:
            return new Cruiser(position, cruiser, 50, 6, 100, true);
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
    //TO DO: sprawdzenie czy liczby nie są za duze albo male (<=0)
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
void Game::displayGame() // TO DO: przerobić ta metode
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
            cout<<"Status: ";
            if (ship->getIsAlive())
            {
                if (ship->canDive())
                {
                    if (ship->isStealth()) {cout<<"Aktywny, pod wodą";}
                    else {cout<<"Aktywny";}
                }
                else {cout<<"Aktywny";}
            }
            else
            {
                cout<<"Zatopiony";
            }
            cout<<"\n";
        }
        else
        {
            cout<<"Brak okrętu pod kursorem.\n";
        }
        cout<<"Wciśnij "<<cancelKey<<", aby wyjść z trybu kursora.\n";
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
        cout<<"Wciśnij "<<cursorModeKey<<", aby wejść w tryb kursora i zobaczyć statystyki i opcje okrętów.\n";
    }

    cout<<"Pozostałe punkty ruchu: "<<currentPlayer->getMovePoints()<<endl;
    cout<<SPLITTER;

    //reszta wiadomosci
    if (currentPlayer->getMovePoints() > 0)
    {
        if (isInCursorMode && !isInMoveMode)
        {
            if (ship != nullptr && ship->getIsAlive() && isCurrentPlayerShip(ship))
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
        //pojedyncza tura
        clearScreen();
        displayGame();
        char key = getKey();

        bool isInCursorMode = board->getIsInCursorMode();
        bool isInMoveMode = board->getIsInMoveMode();
        bool isInAttackMode = board->getIsInAttackMode();

        //wlaczenie kursora
        if (key == cursorModeKey && !isInCursorMode)
        {
            board->toogleCursorMode();
            continue;
        }

        if (key == moveKey)
        {
            if (isInAttackMode) {continue;}

            //potwierdzenie ruchu
            if (isInMoveMode)
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
                        currentPlayer->useMovePoint();
                        continue;
                    }
                }
            }

            //wlaczenie trybu ruchu
            if (isInCursorMode && currentPlayer->getMovePoints() > 0)
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

        if (key == shootAttackKey || key == torpedoAttackKey || key == airStrikeKey)
        {
            if (!isInCursorMode || isInMoveMode) {continue;}

            //potwierdzenie ataku
            if (isInAttackMode)
            {
                Ship* tile = board->getShipUnderCursor();
                if (tile != nullptr && !isCurrentPlayerShip(tile) && tile->getIsAlive())
                {
                    switch (currentAttackType)
                    {
                        case shoot:
                            if (selectedShip->tryShootAttack(tile))
                            {
                                board->toogleAttackMode(nullptr);
                                currentPlayer->useMovePoint();
                                currentAttackType = none;
                            }
                            break;
                        case torpedo:
                            if (selectedShip->tryTorpedoAttack(tile))
                            {
                                board->toogleAttackMode(nullptr);
                                currentPlayer->useMovePoint();
                                currentAttackType = none;
                            }
                            break;
                        case air:
                            
                            if (selectedShip->tryAirStrike(tile))
                            {
                                board->toogleAttackMode(nullptr);
                                currentPlayer->useMovePoint();
                                currentAttackType = none;
                            }
                            break;
                    }
                }
                continue;
            }

            //wlaczenie trybu ataku
            Ship* ship = board->getShipUnderCursor();
            if (ship != nullptr && isCurrentPlayerShip(ship) && ship->getIsAlive() && currentPlayer->getMovePoints() > 0)
            {
                selectedShip = ship;
                switch (key)
                {
                    case shootAttackKey:
                        if (selectedShip->canShoot())
                        {
                            board->toogleAttackMode(selectedShip);
                            currentAttackType = shoot;
                        }
                        break;
                    case torpedoAttackKey:
                        if (selectedShip->canTorpedoAttack())
                        {
                            board->toogleAttackMode(selectedShip);
                            currentAttackType = torpedo;
                        }
                        break;
                    case airStrikeKey:
                        if (selectedShip->canAirStrike())
                        {
                            board->toogleAttackMode(selectedShip);
                            currentAttackType = air;
                        }
                        break;
                }
                continue;
            }
        }

        //wylaczanie trybow
        if (key == cancelKey)
        {
            if (isInMoveMode)
            {
                board->toogleMoveMode(nullptr);
            }
            else if (isInAttackMode)
            {
                board->toogleAttackMode(nullptr);
                currentAttackType = none;
            }
            else if (isInCursorMode)
            {
                board->toogleCursorMode();
            }
            continue;
        }

        //ruch kursorem
        if (isInCursorMode)
        {
            array<int, 2> offset = {0, 0};
            switch (key)
            {
                case moveKeys::up:
                    offset = {0, -1};
                    break;
                case moveKeys::down:
                    offset = {0, 1};
                    break;
                case moveKeys::right:
                    offset = {1, 0};
                    break;
                case moveKeys::left:
                    offset = {-1, 0};
                    break;
                default:
                    break;
            }
            board->moveCursor(offset);
            continue;
        }

        //zmiana tur
        if (key == endTurnKey)
        {
            if (!isInAttackMode && !isInMoveMode && !isInCursorMode)
            {
                switchTurns();
                continue;
            }
        }
    }
}