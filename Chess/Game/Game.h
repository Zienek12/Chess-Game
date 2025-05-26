#pragma once
#include "../HandlePlayerInput/HandlePlayerInput.h"
#include "../Board/Board.h"
#include "../Pieces/Piece.h"
#include "../ConsoleUi/ConsoleUi.h"

class Game
{
protected:
	bool isMoveLegal(const Board& board, const Position& from, const Position& to, Color player, PieceType promotionType) const;

public:
	Color currentPlayer = Color::White;
	static Board initializeGame(const std::string& fen);
	void gameLoop(Board& board);
	bool isGameCompleted(const Board& board, Color player) const;
};