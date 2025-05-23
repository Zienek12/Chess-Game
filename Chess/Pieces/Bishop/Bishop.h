#pragma once
#include "../Piece.h"
#include "../Position.h"
#include "../../Board/Board.h"

class Bishop : public Piece
{
public:
	Bishop(Color color, bool hasMoved = false) : Piece(PieceType::Bishop, color, hasMoved) {}
	std::vector<Position> getLegalMoves(const Position& pos, const Board& board);
};