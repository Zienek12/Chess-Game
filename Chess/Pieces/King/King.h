#pragma once
#include "../Piece.h"
#include "../Position.h"
#include "../../Board/Board.h"
#include "../Move.h"

class King : public Piece
{
public:
	King(Color color, bool hasMoved = false) : Piece(PieceType::King, color, hasMoved) {}
	std::vector<Move> getLegalMoves(const Position& pos, const Board& board);
};