#pragma once
#include "../Pieces/Piece.h"
#include "../Pieces/Position.h"
#include<array>
#include <string>
#include <stdexcept>
#include <map>


class Board
{
protected:
	std::array<std::array <Piece, 8>, 8> squares;
	void initializeFromFEN(const std::string& fenString);
	Piece charToPiece(char c) const;

public:
	Board(const std::string& fenString);
	const Piece& getPiece(const Position& pos) const;
	void placePiece(const Position& pos, const Piece& piece);
	void clearBoard();
	void removePiece(const Position& pos);
	void movePiece(const Position& from, const Position& to);
	std::map<Position, std::vector<Position>> getAllLegalMoves(Color color) const;
	bool isSquareAttacked(const Position& pos, Color color) const;

	Position findKingPos(Color player) const;
	bool isKingInCheck(Color color) const;

	bool isInsufficientMaterial() const;
};