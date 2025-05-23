#include "Board.h"
#include "../Pieces/PieceType.h"
#include "../Pieces/Pawn/Pawn.h"
#include "../Pieces/Knight/Knight.h"
#include "../Pieces/Rook/Rook.h"
#include "../Pieces/Bishop/Bishop.h"
#include <sstream>
#include <cctype>
#include <algorithm>
#include <vector>
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
		const Piece& piece = getPiece(from);
		switch (piece.getType())
		{
		case PieceType::Pawn:
		{
			Pawn pawn(piece.getColor(), piece.hasBeenMoved());
			std::vector<Position> legalPawnMoves = pawn.getLegalMoves(from, *this);
			if (std::find(legalPawnMoves.begin(), legalPawnMoves.end(), to) != legalPawnMoves.end())
			{
				squares[to.x][to.y] = squares[from.x][from.y];
				removePiece(from);
			}
			break;
		}
		case PieceType::Rook:
		{
			Rook rook(piece.getColor(), piece.hasBeenMoved());
			std::vector<Position> legalRookMoves = rook.getLegalMoves(from, *this);
			if (std::find(legalRookMoves.begin(), legalRookMoves.end(), to) != legalRookMoves.end())
			{
				squares[to.x][to.y] = squares[from.x][from.y];
				removePiece(from);
			}
			break;
		}
		case PieceType::Knight:
		{
			Knight knight(piece.getColor(), piece.hasBeenMoved());
			std::vector<Position> legalKnightMoves = knight.getLegalMoves(from, *this);
			if (std::find(legalKnightMoves.begin(), legalKnightMoves.end(), to) != legalKnightMoves.end())
			{
				squares[to.x][to.y] = squares[from.x][from.y];
				removePiece(from);
			}
			break;
		}
		case PieceType::Bishop:
		{
			Bishop bishop(piece.getColor(), piece.hasBeenMoved());
			std::vector<Position> legalBishopMoves = bishop.getLegalMoves(from, *this);
			if (std::find(legalBishopMoves.begin(), legalBishopMoves.end(), to) != legalBishopMoves.end())
			{
				squares[to.x][to.y] = squares[from.x][from.y];
				removePiece(from);
			}
			break;
		}
		case PieceType::Queen:
			// Placeholder for Queen movement logic
			break;
		case PieceType::King:
			// Placeholder for King movement logic
			break;
		case PieceType::None:
			// No action needed for an empty square
			break;
		default:
			throw std::logic_error("Unhandled PieceType in movePiece");
		}

	}
}
