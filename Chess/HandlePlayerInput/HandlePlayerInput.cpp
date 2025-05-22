#include "HandlePlayerInput.h"
#include <iostream>
#include <sstream>
#include "../Pieces/Position.h"
PlayerMove HandlePlayerInput::getPlayerMove() const
{
    std::string input;
    std::cout << "Podaj ruch (np. e2 e4): ";
    PlayerMove move;
    std::cin >> move.from >> move.to;

    return move;
}

std::vector<Position> HandlePlayerInput::translatePlayerMove(PlayerMove move) const
{
    if (move.from.size() != 2 || move.to.size() != 2)
        return {};

    int fromX = move.from[0] - 'a';
    int fromY = move.from[1] - '1';
    int toX = move.to[0] - 'a';
    int toY = move.to[1] - '1';

    return { Position(fromX, fromY), Position(toX, toY) };
}
