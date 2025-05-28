#pragma once
#include "../Pieces/Piece.h"

struct MoveState
{
    Position from;
    Position to;
    Piece movedPiece;
    Piece capturedPiece;
    bool movedPieceHasMoved;
    bool capturedPieceHasMoved;

    bool isCastling = false;
    Position rookFrom;
    Position rookTo;
    Piece rookPiece;
    bool rookHasMoved = false;

    bool isPromotion = false;
    PieceType promotionType = PieceType::None;

    MoveState()
        : from(Position()), to(Position()), movedPiece(Piece()),
        capturedPiece(Piece()), movedPieceHasMoved(false), capturedPieceHasMoved(false),
        isCastling(false), rookFrom(Position()), rookTo(Position()), rookPiece(Piece()), rookHasMoved(false),
        isPromotion(false), promotionType(PieceType::None)
    {
    }
};