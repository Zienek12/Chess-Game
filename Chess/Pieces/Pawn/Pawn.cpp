#include "Pawn.h"

std::vector<Position> Pawn::getLegalMoves(const Position& pos, const Board& board)
{
    int direction = (getColor() == Color::White) ? 1 : -1;
    int startRow = (getColor() == Color::White) ? 1 : 6;

    std::vector<Position> moves;
    Position oneForward = Position(pos.x, pos.y + direction);
    if (oneForward.isValid() && board.getPiece(oneForward).getType() == PieceType::None)
    {
        moves.push_back(oneForward);
        Position twoForward = Position(pos.x, pos.y + direction * 2);
        if (pos.y == startRow && twoForward.isValid() && board.getPiece(twoForward).getType() == PieceType::None)
        {
            moves.push_back(twoForward);
        }
    }
    for (int dx : {-1, 1})
    {
        Position diag(pos.x + dx, pos.y + direction);
        if (diag.isValid())
        {
            const Piece& target = board.getPiece(diag);
            if (target.getType() != PieceType::None && target.getColor() != getColor() && target.getColor() != Color::None)
            {
                moves.push_back(diag);
            }
        }
    }

    return moves;
}
