#include "Rook.h"

std::vector<Move> Rook::getLegalMoves(const Position& pos, const Board& board)
{
	std::vector<Move> legalMoves;

	const Piece& currentPiece = board.getPiece(pos);

	const int dx[] = { 0, 0, -1, 1 };
	const int dy[] = { 1, -1, 0, 0 };

	for (int dir = 0; dir < 4; dir++)
	{
		int newX = pos.x + dx[dir];
		int newY = pos.y + dy[dir];
		while (Position(newX, newY).isValid())
		{
			const Piece& targetPiece = board.getPiece(Position(newX, newY));

			if (targetPiece.getType() == PieceType::None)
			{
				legalMoves.push_back(Move(pos,Position(newX, newY)));
			}
			else
			{
				if (targetPiece.getColor() != currentPiece.getColor())
				{
					legalMoves.push_back(Move(pos,Position(newX, newY)));
				}
				break; 
			}
			newX += dx[dir];
			newY += dy[dir];
		}
	}

	return legalMoves;
}
