#include "../Piece.h"
#include "../Position.h"
#include "../../Board/Board.h"
#include "../Move.h"
class Pawn : public Piece
{
public:
	Pawn(Color color, bool hasMoved = false) : Piece(PieceType::Pawn, color, hasMoved) {}
	std::vector<Move> getLegalMoves(const Position& pos, const Board& board);
};