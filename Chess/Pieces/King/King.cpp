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
    if (!this->hasBeenMoved() && !board.isKingInCheck(this->getColor()))
    {
        int y = (currentPiece.getColor() == Color::White) ? 0 : 7;


        //king side castle
        if (
            board.getPiece(Position(5, y)).getType() == PieceType::None &&
            board.getPiece(Position(6, y)).getType() == PieceType::None &&
            !board.isSquareAttacked(Position(5, y), currentPiece.getColor()) &&
            !board.isSquareAttacked(Position(6, y), currentPiece.getColor())
            )
        {
            const Piece& rook = board.getPiece(Position(7, y));
            if (rook.getType() == PieceType::Rook && !rook.hasBeenMoved() && rook.getColor() == currentPiece.getColor())
            {
                legalMoves.push_back(Position(6, y)); 
            }
        }

    //queen side castle
        if (
            board.getPiece(Position(1, y)).getType() == PieceType::None &&
            board.getPiece(Position(2, y)).getType() == PieceType::None &&
            board.getPiece(Position(3, y)).getType() == PieceType::None &&
            !board.isSquareAttacked(Position(2, y), currentPiece.getColor()) &&
            !board.isSquareAttacked(Position(3, y), currentPiece.getColor())
            )
        {
            const Piece& rook = board.getPiece(Position(0, y));
            if (rook.getType() == PieceType::Rook && !rook.hasBeenMoved() && rook.getColor() == currentPiece.getColor())
            {
                legalMoves.push_back(Position(2, y)); 
            }
        }
    }
    


    return legalMoves;
}
