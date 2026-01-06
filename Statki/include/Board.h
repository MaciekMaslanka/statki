#pragma once
#include <vector>
#include <array>
#include <iostream>
#include "../include/colors.h"
#include "../include/Ship.h"

class Board
{
    private:
        int mapSize;
        std::vector<std::vector<Ship*>> grid;

        Ship* selectedShip = nullptr;
        bool isInCursorMode = false;
        std::array<int, 2> cursorPosition = {0, 0};

        bool isInMoveMode = false;
        bool isInAttackMode = false;
        
        void drawTile(std::string symbol, const std::string& color, bool isCursorHere) const;

    public:
        Board(int size);
        bool isVisible(int x, int y, const std::vector<Ship*>& fleet) const;
        bool isVisible(int x, int y, const Ship* ship) const;
        
        void placeFleet(const std::vector<Ship*>& fleet);
        void updateShipPosition(Ship* ship, std::array<int, 2> newPosition);
        void display(const std::vector<Ship*>& playerFleet);

        void moveCursor(std::array<int, 2> targetPosition);
        bool getIsInCursorMode() const;
        void toogleCursorMode();
        std::array<int, 2> getCursorPosition() const;
        Ship* getShipUnderCursor() const;

        void toogleMoveMode(Ship* ship);
        bool getIsInMoveMode() const;

        void toogleAttackMode(Ship* ship);
        bool getIsInAttackMode() const;

        ~Board();
};