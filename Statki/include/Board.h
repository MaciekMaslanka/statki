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

        bool isVisible(int x, int y, const std::vector<Ship*>& fleet) const;

    public:
        Board(int size);
        void placeFleet(const std::vector<Ship*>& fleet);
        void display(const std::vector<Ship*>& playerFleet);
};