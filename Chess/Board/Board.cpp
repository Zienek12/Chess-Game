#include "Board.h"
#include "../Pieces/PieceType.h"
#include "../Pieces/Pawn/Pawn.h"
#include "../Pieces/Knight/Knight.h"
#include "../Pieces/Rook/Rook.h"
#include "../Pieces/Bishop/Bishop.h"
#include "../Pieces/Queen/Queen.h"
#include "../Pieces/King/King.h"
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


//returns map with position from as a key and vector of position as values to check all legal moves
//for ordered color
std::map<Position, std::vector<Position>> Board::getAllLegalMoves(Color color) const
{
	std::map<Position, std::vector<Position>> moves;
	for (int x = 0; x < 8; ++x)
	{
		for (int y = 0; y < 8; ++y)
		{
			Position from(x, y);
			const Piece& piece = getPiece(from);
			if (piece.getColor() != color || piece.getType() == PieceType::None)
				continue;

			std::vector<Position> legalMoves;
			switch (piece.getType())
			{
			case PieceType::Pawn:
			{
				Pawn p(piece.getColor(), piece.hasBeenMoved());
				legalMoves = p.getLegalMoves(from, *this);
				break;
			}
			case PieceType::Knight:
			{
				Knight k(piece.getColor(), piece.hasBeenMoved());
				legalMoves = k.getLegalMoves(from, *this);
				break;
			}
			case PieceType::Bishop:
			{
				Bishop b(piece.getColor(), piece.hasBeenMoved());
				legalMoves = b.getLegalMoves(from, *this);
				break;
			}
			case PieceType::Rook:
			{
				Rook r(piece.getColor(), piece.hasBeenMoved());
				legalMoves = r.getLegalMoves(from, *this);
				break;
			}
			case PieceType::Queen:
			{
				Queen q(piece.getColor(), piece.hasBeenMoved());
				legalMoves = q.getLegalMoves(from, *this);
				break;
			}
			case PieceType::King:
			{
				King k(piece.getColor(), piece.hasBeenMoved());
				legalMoves = k.getLegalMoves(from, *this);
				break;
			}
			default:
				break;
			}
			if (!legalMoves.empty())
				moves[from] = legalMoves;
		}
	}
	return moves;
}


//uses map with all legal moves it checs if move that player inputed is in the map
//if it is move is made
void Board::movePiece(const Position& from, const Position& to)
{
	if (!from.isValid() || !to.isValid())
		return;

	const Piece& piece = getPiece(from);
	if (piece.getType() == PieceType::None)
		return;

	auto allMoves = getAllLegalMoves(piece.getColor());
	auto it = allMoves.find(from);
	if (it != allMoves.end())
	{
		const std::vector<Position>& legalMoves = it->second;
		if (std::find(legalMoves.begin(), legalMoves.end(), to) != legalMoves.end())
		{
			squares[to.x][to.y] = squares[from.x][from.y];
			removePiece(from);
		}
	}
}

