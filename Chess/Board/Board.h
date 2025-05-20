#pragma once
#include "../Pieces/Piece.h"
#include "../Pieces/Position.h"
#include<array>
#include <string>
#include <stdexcept>


class Board
{
private:
	std::array<std::array <Piece, 8>, 8> squares;
	void initializeFromFEN(const std::string& fenString);
	Piece charToPiece(char c) const;

public:
	Board();
	Board(const std::string& fenString);
	const Piece& getPiece(const Position& pos) const;
	void placePiece(const Position& pos, const Piece& piece);
	void clearBoard();
	void loadFromFEN(const std::string& fenString); 
};