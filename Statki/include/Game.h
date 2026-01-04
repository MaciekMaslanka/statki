#pragma once
#include <iostream>
#include <array>
#include <vector>
#include "../include/Board.h"
#include "../include/Player.h"
#include "../include/Ship.h"
#include "../include/Submarine.h"
#include "../include/AircraftCarrier.h"
#include "../include/Destroyer.h"
#include "../include/Cruiser.h"
#include "../include/platform.h"

enum attackTypes{none, air, shoot, torpedo};

class Game
{
    private:
        Board* board = nullptr;
        Player* player1 = nullptr;
        Player* player2 = nullptr;
        Player* currentPlayer = nullptr;
        Ship* selectedShip = nullptr;
        attackTypes currentAttackType = none;

        void switchTurns();
        void placePlayerShips(const int shipsAmount[4], Player* player1, int mapSize, int xMax, int xMin);
        bool isOccupied(std::array<int, 2> pos, const std::vector<std::array<int, 2>>& occupiedPositions);
        bool isCurrentPlayerShip(Ship* ship) const;
        void displayGame();
        
    public:
        Game();
        Ship* createShip(shipType type, std::array<int, 2> position);
        
        //rozgrywka
        void beginGame();
        void gameLoop();

        ~Game() = default;
};