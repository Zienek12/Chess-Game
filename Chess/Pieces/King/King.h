#pragma once
#include "../Piece.h"
#include "../Position.h"
#include "../../Board/Board.h"

class King : public Piece
{
public:
	King(Color color, bool hasMoved = false) : Piece(PieceType::King, color, hasMoved) {}
	std::vector<Position> getLegalMoves(const Position& pos, const Board& board);
};