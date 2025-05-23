#pragma once
#include "../Piece.h"
#include "../Position.h"
#include "../../Board/Board.h"

class Knight : public Piece
{
public:
	Knight(Color color, bool hasMoved = false) : Piece(PieceType::Knight, color, hasMoved) {}
	std::vector<Position> getLegalMoves(const Position& pos, const Board& board);
};