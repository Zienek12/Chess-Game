#pragma once
#include "../Piece.h"
#include "../Position.h"
#include "../../Board/Board.h"
#include "../Move.h"

class Queen : public Piece
{
public:
	Queen(Color color, bool hasMoved = false) : Piece(PieceType::Queen, color, hasMoved) {}
	std::vector<Move> getLegalMoves(const Position& pos, const Board& board);
};