#include "HandlePlayerInput.h"
#include <iostream>
#include <sstream>
#include "../Pieces/Position.h"
PlayerMove HandlePlayerInput::getPlayerMove() const
{
    PlayerMove move;
    while (true)
    {
        std::cout << "Podaj ruch (np. e2 e4): ";
        std::cin >> move.from >> move.to;

        if (move.from.size() == 2 && move.to.size() == 2 &&
            move.from[0] >= 'a' && move.from[0] <= 'h' &&
            move.from[1] >= '1' && move.from[1] <= '8' &&
            move.to[0] >= 'a' && move.to[0] <= 'h' &&
            move.to[1] >= '1' && move.to[1] <= '8')
        {
            break;
        }
        std::cout << "Incorrect move format! Please try again\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
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
