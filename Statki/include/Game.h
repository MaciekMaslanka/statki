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
#include "../include/GameMessage.h"

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
        std::vector<GameMessage> messages;
        std::vector<std::string> actionHints;

        void switchTurns();
        void updateActionHints();
        Ship* createShip(shipType type, std::array<int, 2> position);
        void placePlayerShips(const int shipsAmount[4], Player* player1, int mapSize, int xMax, int xMin);
        bool isOccupied(std::array<int, 2> pos, const std::vector<std::array<int, 2>>& occupiedPositions);
        bool isCurrentPlayerShip(Ship* ship) const;
        bool checkForEndGame();
        void endGame(Player* winner);
        void displayGame();
        
    public:
        Game();
        //rozgrywka
        void beginGame();
        void gameLoop();

        ~Game();
};