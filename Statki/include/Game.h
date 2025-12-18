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

class Game
{
    private:
        Board* board = nullptr;
        Player* player1 = nullptr;
        Player* player2 = nullptr;

        void switchTurns();
        void placeShips(int shipsAmount[4], Player* player1, Player* player2, int mapSize);
        bool isOccupied(std::array<int, 2> pos, std::vector<std::array<int, 2>>& occupiedPositions);
        
    public:
        Game();
        Ship* createShip(shipType type, std::array<int, 2> position);
        
        //rozgrywka
        void beginGame();
        void gameLoop();

        ~Game() = default;
};