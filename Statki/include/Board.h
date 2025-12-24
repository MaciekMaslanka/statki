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

        bool isInCursorMode = false;
        std::array<int, 2> cursorPosition = {0, 0};

        bool isVisible(int x, int y, const std::vector<Ship*>& fleet) const;
        void drawTile(std::string symbol, const std::string& color, bool isCursorHere) const;

    public:
        Board(int size);
        void placeFleet(const std::vector<Ship*>& fleet);
        void display(const std::vector<Ship*>& playerFleet);

        void moveCursor(std::array<int, 2> targetPosition);
        bool getIsInCursorMode() const;
        void toogleCursorMode();
        std::array<int, 2> getCursorPosition() const;
        Ship* getShipUnderCursor() const;

        ~Board() = default;
};