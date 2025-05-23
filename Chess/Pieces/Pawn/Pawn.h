#pragma once
#include "../Piece.h"
#include "../Position.h"
#include "../../Board/Board.h"
class Pawn : public Piece
{
public:
	Pawn(Color color, bool hasMoved = false) : Piece(PieceType::Pawn, color, hasMoved) {}
	std::vector<Position> getLegalMoves(const Position& pos, const Board& board);
};