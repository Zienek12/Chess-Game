#include "Queen.h"


std::vector<Position> Queen::getLegalMoves(const Position& pos, const Board& board)
{

	std::vector<Position> legalMoves;
	const Piece& currentPiece = board.getPiece(pos);

	const int dx[] = { -1, 1, -1, 1, 0, 0, 1, -1};
	const int dy[] = { 1, -1, -1, 1, 1, -1, 0, 0};

	for (int dir = 0; dir < 8; dir++)
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
				break;
			}
			newX += dx[dir];
			newY += dy[dir];
		}
	}

	return legalMoves;

}