#pragma once
#include "../Piece.h"
#include "../Position.h"
#include "../../Board/Board.h"

class Queen : public Piece
{
public:
	Queen(Color color, bool hasMoved = false) : Piece(PieceType::Queen, color, hasMoved) {}
	std::vector<Position> getLegalMoves(const Position& pos, const Board& board);
};