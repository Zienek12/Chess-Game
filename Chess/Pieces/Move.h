#pragma once
#include "PieceType.h"
#include "Position.h"

struct Move {
    Position from;
    Position to;
    PieceType promotionType = PieceType::None;
    
    Move() : from(Position()), to(Position()), promotionType(PieceType::None) {}
    Move(const Position& from, const Position& to, PieceType promotionType = PieceType::None)
        : from(from), to(to), promotionType(promotionType) {
    }
};