#include "Pawn.h"

std::vector<Move> Pawn::getLegalMoves(const Position& pos, const Board& board)
{
    int direction = (getColor() == Color::White) ? 1 : -1;
    int startRow = (getColor() == Color::White) ? 1 : 6;
    int promotionRow = (getColor() == Color::White) ? 7 : 0;
    std::vector<Move> moves;

    //Move one forward
    Position oneForward = Position(pos.x, pos.y + direction);
    if (oneForward.isValid() && board.getPiece(oneForward).getType() == PieceType::None)
    {
        if (oneForward.y == promotionRow)
        {
            moves.emplace_back(pos, oneForward, PieceType::Queen);
            moves.emplace_back(pos, oneForward, PieceType::Rook);
            moves.emplace_back(pos, oneForward, PieceType::Bishop);
            moves.emplace_back(pos, oneForward, PieceType::Knight);
        }
        else
        {
            moves.emplace_back(pos, oneForward);
        }

        // Move two forward if on starting position
        Position twoForward = Position(pos.x, pos.y + direction * 2);
        if (pos.y == startRow && twoForward.isValid() && board.getPiece(twoForward).getType() == PieceType::None)
        {
            moves.emplace_back(pos, twoForward);
        }
    }

    // taking diagonally
    for (int dx : {-1, 1})
    {
        Position diag(pos.x + dx, pos.y + direction);
        if (diag.isValid())
        {
            const Piece& target = board.getPiece(diag);
            if (target.getType() != PieceType::None && target.getColor() != getColor() && target.getColor() != Color::None)
            {
                if (diag.y == promotionRow)
                {
                    for (PieceType promo : {PieceType::Queen, PieceType::Rook, PieceType::Bishop, PieceType::Knight})
                    {
                        moves.emplace_back(pos, diag, promo);
                    }
                }
                else
                {
                    moves.emplace_back(pos, diag);
                }
            }
        }
    }

    // NYI en passante

    return moves;
}
