#include "Rook.h"

std::vector<Position> Rook::getLegalMoves(const Position& pos, const Board& board)
{
	std::vector<Position> legalMoves;

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
				legalMoves.push_back(Position(newX, newY));
			}
			else
			{
				if (targetPiece.getColor() != currentPiece.getColor())
				{
					legalMoves.push_back(Position(newX, newY));
				}
				break; // nie mo?na przej?? przez inn? figur?
			}
			newX += dx[dir];
			newY += dy[dir];
		}
	}

	return legalMoves;
}
