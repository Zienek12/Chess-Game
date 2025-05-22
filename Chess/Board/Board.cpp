#include "Board.h"
#include "../Pieces/PieceType.h"
#include <sstream>
#include <cctype>

const Piece& Board::getPiece(const Position& pos)const {
	if (!pos.isValid())
	{
		static Piece empty;
		return empty;
	}
	return squares[pos.x][pos.y];
}
void Board::placePiece(const Position& pos, const Piece& piece)
{
	if (pos.isValid())
	{
		squares[pos.x][pos.y] = piece;
	}
}

void Board::clearBoard()
{
	for (auto& row : squares)
	{
		for (auto& square : row)
		{
			square = Piece();
		}
	}
}


Board::Board(const std::string& fenString)
{
	initializeFromFEN(fenString);
}

void Board::loadFromFEN(const std::string& fenString) {
	initializeFromFEN(fenString);
}

void Board::initializeFromFEN(const std::string& fenString)
{
	clearBoard();
	std::istringstream fenStream(fenString);
	std::string piecePlacement;
	fenStream >> piecePlacement;
	int x = 0, y = 7;
	for (char c : piecePlacement)
	{
		if (c == '/')
		{
			//nowa linia
			x = 0;
			y--;
		}
		else if (isdigit(c))
		{
			//puste pola
			x += (c - '0');
		}
		else
		{
			//figura
			if (x >= 8 || y < 0)
			{
				throw std::invalid_argument("Incorrect FEN notation: too many characters");
			}
			placePiece(Position(x, y), charToPiece(c));
			x++;
		}
	}
}

Piece Board::charToPiece(char c) const
{
	Color color = isupper(c) ? Color::White : Color::Black;
	char lowerC = static_cast<char>(tolower(c));

	PieceType type;
	switch (lowerC)
	{
	case 'p': type = PieceType::Pawn; break;
	case 'q': type = PieceType::Queen; break;
	case 'k': type = PieceType::King; break;
	case 'r': type = PieceType::Rook; break;
	case 'b': type = PieceType::Bishop; break;
	case 'n': type = PieceType::Knight; break;
	default:
		throw std::invalid_argument("Unknown figure symbol in FEN: " + std::string(1, c));
	}

	return Piece(type, color);
}



void Board::removePiece(const Position& pos)
{
	if (pos.isValid())
	{
		squares[pos.x][pos.y] = Piece(PieceType::None,Color::None);
	}
}

void Board::movePiece(const Position& from, const Position& to)
{
	if (from.isValid() && to.isValid())
	{
		squares[to.x][to.y] = squares[from.x][from.y];
		removePiece(Position(from.x, from.y));
	}
}
