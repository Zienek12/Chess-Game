#pragma once
#include "../Piece.h"
#include "../Position.h"
#include "../../Board/Board.h"
#include "../Move.h"

class Knight : public Piece
{
public:
	Knight(Color color, bool hasMoved = false) : Piece(PieceType::Knight, color, hasMoved) {}
	std::vector<Move> getLegalMoves(const Position& pos, const Board& board);
};