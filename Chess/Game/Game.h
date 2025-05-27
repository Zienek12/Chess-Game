#pragma once
#include "../HandlePlayerInput/HandlePlayerInput.h"
#include "../Board/Board.h"
#include "../Pieces/Piece.h"
#include "../ConsoleUi/ConsoleUi.h"
#include "../ChessEngine/ChessEngine.h"
#include <Windows.h>
#include <algorithm>
class Game
{
protected:
	bool isMoveLegal(const Board& board, const Position& from, const Position& to, Color player, PieceType promotionType) const;

public:
	int counter = 1;
	Color currentPlayer = Color::White;
	Color playerColor = Color::None;
	Color aiColor = Color::None;
	Board initializeGame(const std::string& fen);
	void gameLoop(Board& board);
	bool isGameCompleted(const Board& board) const;
};