#pragma once
#include <string>
#include "../Pieces/Position.h"
#include <vector>
struct PlayerMove
{
	std::string from;
	std::string to;
};

class HandlePlayerInput
{
public:
	PlayerMove getPlayerMove() const;
	std::vector<Position> translatePlayerMove(PlayerMove move) const;

};