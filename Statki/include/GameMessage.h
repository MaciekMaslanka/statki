#pragma once
#include <string>

enum class MessageTypes{info, warning, error};
struct GameMessage
{
    std::string text;
    MessageTypes type;
};