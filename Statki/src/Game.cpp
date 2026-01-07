#include "../include/Game.h"
#include <iostream>
#include <cmath>

using namespace std;

const string SPLITTER = "\n-------------------------------------\n";

//config
const char cursorModeKey = 'c';
const char moveKey = 'm';
const char diveKey = 'n';
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

bool Game::checkForEndGame()
{
    Player* otherPlayer;
    if (currentPlayer == player1) {otherPlayer = player2;}
    else {otherPlayer = player1;}

    vector<Ship*> fleet = otherPlayer->getFleet();
    for (Ship* ship : fleet)
    {
        if (ship->getIsAlive())
        {
            return false;
        }
    }
    //jesli wszystkie statki zniszczone
    return true;
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

Ship* Game::createShip(shipType type, array<int, 2> position)
{
    switch (type)
    {
        case submarine:
            return new Submarine(position, submarine, 20, 3, 15, true, 6, 10, 3);
        case aCarrier:
            return new AircraftCarrier(position, aCarrier, 15, 10, 40, true, 60, 15);
        case destroyer:
            return new Destroyer(position, destroyer, 25, 5, 20, true, 4, 6, 10);
        case cruiser:
            return new Cruiser(position, cruiser, 18, 7, 30, true, 6);
        default:
            return nullptr;
    }
}
void Game::updateActionHints()
{
    actionHints.clear();

    if (!board->getIsInCursorMode())
    {
        actionHints.push_back(string(1, cursorModeKey)+"- wejdz w tryb kursora");
        actionHints.push_back(string(1, endTurnKey)+"- zakoncz ture");
        return;
    }
    else
    {
        actionHints.push_back("w s a d- ruch kursorem");
        actionHints.push_back(string(1, cancelKey)+"- wylacz kursor");
        if (board->getIsInMoveMode())
        {
            actionHints.push_back(string(1, moveKey)+"- potwierdz ruch");
            actionHints.push_back(string(1, cancelKey)+"- anuluj");
            return;
        }
        if (board->getIsInAttackMode())
        {
            switch (currentAttackType)
            {
                case attackTypes::air:
                    actionHints.push_back(string(1, airStrikeKey)+"- potwierdz atak");
                    break;
                case attackTypes::shoot:
                    actionHints.push_back(string(1, shootAttackKey)+"- potwierdz atak");
                    break;
                case attackTypes::torpedo:
                    actionHints.push_back(string(1, torpedoAttackKey)+"- potwierdz atak");
                    break;
                default: break;
            }
            actionHints.push_back(string(1, cancelKey)+"- anuluj");
            return;
        }

        if (selectedShip != nullptr && isCurrentPlayerShip(selectedShip) && selectedShip->getIsAlive())
        {
            actionHints.push_back(string(1, moveKey)+"- ruch");
            if (selectedShip->canShoot())
            {
                actionHints.push_back(string(1, shootAttackKey)+"- ostrzal");
            }
            if (selectedShip->canTorpedoAttack())
            {
                actionHints.push_back(string(1, torpedoAttackKey)+"- torpeda");
            }
            if (selectedShip->canAirStrike())
            {
                actionHints.push_back(string(1, airStrikeKey)+"- nalot");
            }
            if (selectedShip->canDive())
            {
                actionHints.push_back(string(1, diveKey)+"- wynurzenie/zanurzenie");
            }
            return;
        }
        else
        {
            actionHints.push_back("Brak opcji");
            return;
        }
    }
    
}

Game::Game() {}

//rozgrywka
void Game::beginGame()
{
    srand(time(NULL));
    string name1, name2;
    int shipsAmount[4] = {0, 0, 0, 0}; //podwodne, lotniskowce, niszczyciele, krazowniki
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
            cout<<"Nieprawidlowy rozmiar mapy. Sprobuj ponownie.\n";
        }
    } while (mapSize < 20 || mapSize > 50);
    board = new Board(mapSize);

    //flota

    if (mapSize <= 30)
    {
        shipsAmount[0] = 1; //podwodne
        shipsAmount[1] = 1; //lotniskowce
        shipsAmount[2] = 2; //niszczyciele
        shipsAmount[3] = 2; //krazowniki
    }
    else
    {
        shipsAmount[0] = 2;
        shipsAmount[1] = 1;
        shipsAmount[2] = 3;
        shipsAmount[3] = 2;
    }

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
    cout<<"Statystyki wybranego statku:\n";
    if (selectedShip != nullptr)
    {
        bool isAlive = selectedShip->getIsAlive();
        bool isUnderwater = selectedShip->isStealth();
        bool isThisPlayerShip = isCurrentPlayerShip(selectedShip);
        float health = selectedShip->getHealth();
        float initHealth = selectedShip->getInitialHealth();
        int fuel = selectedShip->getFuelAmount();
        int initFuel = selectedShip->getInitialFuelAmount();

        if (isAlive && isThisPlayerShip)
        {
            cout<<"Status: ";
            if (isUnderwater) {cout<<"Aktywny, pod woda\n";}
            else {cout<<"Aktywny\n";}
            cout<<"HP: "<<round((double)health / initHealth * 100)<<"% ("<<health<<"/"<<initHealth<<")\n";
            cout<<"Paliwo: "<<round((double)fuel / initFuel * 100)<<"% ("<<fuel<<"/"<<initFuel<<")\n";
            if (selectedShip->canTorpedoAttack())
            {
                int initTorpedo = selectedShip->getInitialTorpedoesAmount();
                int torpedo = selectedShip->getTorpedoesAmount();
                cout<<"Torpedy: "<<torpedo<<"/"<<initTorpedo<<"\n";
            }
            if (selectedShip->canAirStrike())
            {
                int initAircraft = selectedShip->getInitialAircraftAmount();
                int aircraft = selectedShip->getAircraftAmount();
                cout<<"Samoloty: "<<aircraft<<"/"<<initAircraft<<"\n";
            }
            if (selectedShip->canDive())
            {
                cout<<"Tury pod woda: "<<selectedShip->getTurnsUnderwater()<<"\n";
            }
            cout<<"Czy juz atakowal: "<<(selectedShip->getHasAttackedThisTurn() == true ? "Tak" : "Nie")<<"\n";
        }
        else if (!isThisPlayerShip)
        {
            array<int, 2> pos = selectedShip->getPosition();
            if (board->isVisible(pos[0], pos[1], currentPlayer->getFleet()))
            {
                if (isAlive && !isUnderwater) 
                {
                    cout<<"Status: Aktywny\n";
                    cout<<"HP: "<<round((double)health / initHealth * 100)<<"% ("<<health<<"/"<<initHealth<<")\n";
                    cout<<"Paliwo: ??\n";
                    cout<<"Czy juz atakowal: "<<(selectedShip->getHasAttackedThisTurn() == true ? "Tak" : "Nie")<<"\n";
                }
                else if (!isAlive) {cout<<"Status: Zniszczony\n";}
                else if (isUnderwater) {cout<<"Brak wybranego statku\n";}
            }
            else
            {
                cout<<"Brak wybranego statku\n";
            }

        }
        else {cout<<"Zniszczony\n";}
    }
    else
    {
        cout<<"Brak wybranego statku\n";
    }
    cout<<"Pozostale punkty ruchu: "<<currentPlayer->getMovePoints()<<"\n";
    //reszta wiadomosci
    cout<<SPLITTER;
    for (GameMessage msg : messages)
    {
        switch (msg.type)
        {
            case MessageTypes::info:
                cout<<"[INFO] ";
                break;
            case MessageTypes::warning:
                cout<<YELLOW<<"[OSTRZEZENIE] ";
                break;
            case MessageTypes::error:
                cout<<RED<<"[ERROR] ";
                break;
            default:
                break;
        }
        cout<<msg.text<<RESET<<"\n";
    }
    cout<<"\n";
    for (string hint : actionHints)
    {
        cout<<hint<<"\n";
    }

}
void Game::gameLoop()
{
    while (true)
    {
        //pojedyncza klatka

        updateActionHints();
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

        //ruch
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
                        messages.clear();
                        messages.emplace_back("Ruch wykonany, zurzyto "+to_string(moveCost)+" jednostek paliwa", MessageTypes::info);
                        board->updateShipPosition(selectedShip, targetPos);
                        board->toogleMoveMode(nullptr);
                        currentPlayer->useMovePoint();
                        continue;
                    }
                    else if (moveCost = -1)
                    {
                        messages.clear();
                        messages.emplace_back("Wybierz cel ruchu", MessageTypes::info);
                        messages.emplace_back("Ruch nieudany, cel jest za daleko", MessageTypes::warning);
                        continue;
                    }
                    else
                    {
                        messages.clear();
                        messages.emplace_back("Wybierz cel ruchu", MessageTypes::info);
                        messages.emplace_back("Ruch nieudany, nie masz punktow ruchu", MessageTypes::warning);
                        continue;
                    }
                }
                else
                {
                    messages.clear();
                    messages.emplace_back("Wybierz cel ruchu", MessageTypes::info);
                    messages.emplace_back("Ruch nieudany, ta kratka jest juz zajeta", MessageTypes::warning);
                    continue;
                }
            }

            //wlaczenie trybu ruchu
            if (isInCursorMode && currentPlayer->getMovePoints() > 0)
            {
                if (selectedShip != nullptr && isCurrentPlayerShip(selectedShip) && selectedShip->getIsAlive())
                {
                    messages.emplace_back("Wybierz cel ruchu", MessageTypes::info);
                    board->toogleMoveMode(selectedShip);
                }
                continue;
            }
        }

        //atak
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
                                messages.clear();
                                messages.emplace_back("Ostrzal udany", MessageTypes::info);
                            }
                            else
                            {
                                messages.clear();
                                messages.emplace_back("Wybierz cel do ostrzalu", MessageTypes::info);
                                messages.emplace_back("Ostrzal nieudany", MessageTypes::warning);
                            }
                            break;
                        case torpedo:
                            if (selectedShip->tryTorpedoAttack(tile))
                            {
                                board->toogleAttackMode(nullptr);
                                currentPlayer->useMovePoint();
                                currentAttackType = none;
                                messages.clear();
                                messages.emplace_back("Atak torpeda udany", MessageTypes::info);
                            }
                            else
                            {
                                messages.clear();
                                messages.emplace_back("Wybierz cel torpedy", MessageTypes::info);
                                messages.emplace_back("Atak torpeda nieudany", MessageTypes::warning);
                            }
                            break;
                        case air:
                            
                            if (selectedShip->tryAirStrike(tile))
                            {
                                board->toogleAttackMode(nullptr);
                                currentPlayer->useMovePoint();
                                currentAttackType = none;
                                messages.clear();
                                messages.emplace_back("Nalot udany", MessageTypes::info);
                            }
                            else
                            {
                                messages.clear();
                                messages.emplace_back("Wybierz cel do nalotu", MessageTypes::info);
                                messages.emplace_back("Nalot nieudany", MessageTypes::warning);
                            }
                            break;
                    }
                    if (checkForEndGame()) {break;}
                }
                else if (tile == nullptr)
                {
                    messages.clear();
                    messages.emplace_back("Wybierz cel do nalotu", MessageTypes::info);
                    messages.emplace_back("Atak nieudany, brak wybranego celu", MessageTypes::warning);
                }
                else if (isCurrentPlayerShip(tile))
                {
                    messages.clear();
                    messages.emplace_back("Wybierz cel do nalotu", MessageTypes::info);
                    messages.emplace_back("Atak nieudany, cel to twoj statek", MessageTypes::warning);
                }
                else
                {
                    messages.clear();
                    messages.emplace_back("Wybierz cel do nalotu", MessageTypes::info);
                    messages.emplace_back("Atak nieudany, cel jest juz zniszczony", MessageTypes::warning);
                }
                continue;
            }

            //wlaczenie trybu ataku
            if (selectedShip != nullptr && isCurrentPlayerShip(selectedShip) && selectedShip->getIsAlive() 
            && currentPlayer->getMovePoints() > 0 && !selectedShip->getHasAttackedThisTurn())
            {
                switch (key)
                {
                    case shootAttackKey:
                        if (selectedShip->canShoot())
                        {
                            messages.clear();
                            messages.emplace_back("Wybierz cel do ostrzalu", MessageTypes::info);
                            board->toogleAttackMode(selectedShip);
                            currentAttackType = shoot;
                        }
                        break;
                    case torpedoAttackKey:
                        if (selectedShip->canTorpedoAttack())
                        {
                            messages.clear();
                            messages.emplace_back("Wybierz cel torpedy", MessageTypes::info);
                            board->toogleAttackMode(selectedShip);
                            currentAttackType = torpedo;
                        }
                        break;
                    case airStrikeKey:
                        if (selectedShip->canAirStrike())
                        {
                            messages.clear();
                            messages.emplace_back("Wybierz cel do nalotu", MessageTypes::info);
                            board->toogleAttackMode(selectedShip);
                            currentAttackType = air;
                        }
                        break;
                }
                continue;
            }
            else if (selectedShip->getHasAttackedThisTurn())
            {
                messages.clear();
                messages.emplace_back("Ten statek już atakował podczas tej tury!", MessageTypes::warning);
            }
        }

        //zanurzenie
        if (key == diveKey)
        {
            if (selectedShip != nullptr && selectedShip->getIsAlive() && isCurrentPlayerShip(selectedShip) && currentPlayer->getMovePoints() > 0)
            {
                if (selectedShip->canDive() && selectedShip->getTurnsUnderwater() > 0)
                {
                    messages.clear();
                    if (selectedShip->isStealth())
                    {
                        selectedShip->leaveStealth();
                        messages.emplace_back("Statek sie wynurzyl", MessageTypes::info);
                    }
                    else
                    {
                        selectedShip->enterStealth();
                        messages.emplace_back("Statek sie zanurzyl", MessageTypes::info); 
                    }
                    currentPlayer->useMovePoint();
                }
                else if (selectedShip->getTurnsUnderwater() <= 0)
                {
                    messages.clear();
                    messages.emplace_back("Ten statek nie moze sie zanurzyc.", MessageTypes::warning);
                }
            }
            continue;;
        }

        //wylaczanie trybow
        if (key == cancelKey)
        {
            if (isInMoveMode)
            {
                messages.clear();
                messages.emplace_back("Anulowano ruch", MessageTypes::info);
                board->toogleMoveMode(nullptr);
            }
            else if (isInAttackMode)
            {
                messages.clear();
                messages.emplace_back("Anulowano atak", MessageTypes::info);
                board->toogleAttackMode(nullptr);
                currentAttackType = none;
            }
            else if (isInCursorMode)
            {
                messages.clear();
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
            if (!isInAttackMode && !isInMoveMode)
            {
                selectedShip = board->getShipUnderCursor();
            }
            continue;
        }

        //zmiana tur
        if (key == endTurnKey)
        {
            if (!isInAttackMode && !isInMoveMode && !isInCursorMode)
            {
                switchTurns();
                board->placeCursorAtCenter();
                messages.clear();
                continue;
            }
        }
    }
    //zakonczenie gry
    endGame(currentPlayer);
}
void Game::endGame(Player* winner)
{
    clearScreen();
    cout<<"Wygrywa gracz "<<winner->getName()<<"\n";
    cout<<"Gratulacje!\n\n";
}
Game::~Game()
{
    delete board;
    delete player1;
    delete player2;
}