#include "Knight.h"

std::vector<Position> Knight::getLegalMoves(const Position& pos, const Board& board)
{
    std::vector<Position> legalMoves;
    static const std::vector<std::pair<int, int>> moves = {
      {2, 1}, {1, 2}, {-1, 2}, {-2, 1},
      {-2, -1}, {-1, -2}, {1, -2}, {2, -1}
    };
    const Piece& currentPiece = board.getPiece(pos);

    for (const auto& move : moves)
    {
        int newX = pos.x + move.first;
        int newY = pos.y + move.second;
        Position newPos(newX, newY);

        if (newPos.isValid())
        {
            const Piece& targetPiece = board.getPiece(newPos);
            if (targetPiece.getType() == PieceType::None || targetPiece.getColor() != currentPiece.getColor())
            {
                legalMoves.push_back(newPos);
            }
        }
    }

    return legalMoves;
}