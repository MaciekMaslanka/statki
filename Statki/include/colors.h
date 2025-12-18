#pragma once
#include <string>

//kolory ansi
const std::string RESET = "\033[0m";

const std::string DARK_BLUE_WATER = "\033[90m~\033[0m";
const std::string LIGHT_BLUE_WATER = "\033[94m~\033[0m";

const std::string ENEMY_SHIP = "\033[31m";
const std::string SELECTED_ENEMY_SHIP = "\033[91m";
const std::string PLAYER_SHIP = "\033[92m";
const std::string UNDERWATER_PLAYER_SHIP = "\033[32m";
const std::string SELECTED_PLAYER_SHIP = "\033[96m";
const std::string DESTROYED = "\033[93m🔥\033[0m";