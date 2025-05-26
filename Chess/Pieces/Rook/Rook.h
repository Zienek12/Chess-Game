#pragma once
#include "../Piece.h"
#include "../Position.h"
#include "../../Board/Board.h"
#include "../Move.h"

class Rook : public Piece
{
public:
	Rook(Color color, bool hasBeenMoved = false) : Piece(PieceType::Rook, color, hasBeenMoved) {}
	std::vector<Move> getLegalMoves(const Position& pos, const Board& board);
};