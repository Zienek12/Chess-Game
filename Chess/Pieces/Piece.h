#pragma
#include "PieceType.h"
#include "Color.h"


class Piece
{
private:
	PieceType type;
	Color color;
	bool hasMoved;

public:
	Piece(PieceType type = PieceType::None, 
		Color color = Color::None, bool hasMoved = false) : type(type), color(color), hasMoved(hasMoved) {}

	PieceType getType() const { return type; }
	Color getColor() const { return color; }
	bool hasBeenMoved() const { return hasMoved; }
};