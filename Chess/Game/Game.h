#pragma once
#include "../HandlePlayerInput/HandlePlayerInput.h"
#include "../Board/Board.h"
#include "../Pieces/Piece.h"
#include "../ConsoleUi/ConsoleUi.h"

class Game
{
protected:
	Color currentPlayer = Color::White;
	bool isMoveLegal(const Board& board, const Position& from, const Position& to, Color player) const;

public:
	static Board initializeGame(const std::string& fen);
	void gameLoop(Board& board);
};