#include "King.h"

std::vector<Position> King::getLegalMoves(const Position& pos, const Board& board)
{
    std::vector<Position> legalMoves;
    const Piece& currentPiece = board.getPiece(pos);

    const int dx[] = { -1, -1, -1, 0, 0, 1, 1, 1 };
    const int dy[] = { -1, 0, 1, -1, 1, -1, 0, 1 };

    for (int dir = 0; dir < 8; dir++)
    {
        int newX = pos.x + dx[dir];
        int newY = pos.y + dy[dir];
        Position newPos(newX, newY);
        if (newPos.isValid())
        {
            const Piece& targetPiece = board.getPiece(newPos);
            if (targetPiece.getType() == PieceType::None ||
                targetPiece.getColor() != currentPiece.getColor())
            {
                if (!board.isSquareAttacked(newPos, currentPiece.getColor()))
                {
                    legalMoves.push_back(newPos);
                }
            }
        }
    }
    return legalMoves;
}
