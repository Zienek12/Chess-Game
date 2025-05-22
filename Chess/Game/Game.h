#pragma once
#include "../HandlePlayerInput/HandlePlayerInput.h"
#include "../Board/Board.h"
#include "../Pieces/Piece.h"
#include "../ConsoleUi/ConsoleUi.h"

class Game
{
public:
	static Board initializeGame(const std::string& fen);
	static void gameLoop(Board& board);
};