#pragma once

#include "../Pieces/Piece.h"

// MoveState stores all necessary information to undo a move on the chessboard.
struct MoveState
{
    // Source and destination positions of the move.
    Position from;
    Position to;

    // The piece that was moved and the piece that was captured (if any).
    Piece movedPiece;
    Piece capturedPiece;

    // Flags indicating whether the moved or captured piece had previously moved.
    bool movedPieceHasMoved;
    bool capturedPieceHasMoved;

    // Castling information
    bool isCastling = false;
    Position rookFrom;
    Position rookTo;
    Piece rookPiece;
    bool rookHasMoved = false;

    // Promotion information
    bool isPromotion = false;
    PieceType promotionType = PieceType::None;

    // Default constructor initializes all members.
    MoveState()
        : from(Position()), to(Position()), movedPiece(Piece()),
        capturedPiece(Piece()), movedPieceHasMoved(false), capturedPieceHasMoved(false),
        isCastling(false), rookFrom(Position()), rookTo(Position()), rookPiece(Piece()), rookHasMoved(false),
        isPromotion(false), promotionType(PieceType::None)
    {
    }
};
