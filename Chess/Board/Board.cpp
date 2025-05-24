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


			std::vector<Position> trulyLegalMoves;
			for (const Position& to : legalMoves)
			{
				Board boardCopy = *this;
				boardCopy.makeMoveNoValidation(from, to);
				if (!boardCopy.isKingInCheck(color))
				{
					trulyLegalMoves.push_back(to);
				}
			}

			if (!trulyLegalMoves.empty())
				moves[from] = trulyLegalMoves;
		}
	}
	return moves;
}

void Board::makeMoveNoValidation(const Position& from, const Position& to)
{
	if (!from.isValid() || !to.isValid())
		return;
	squares[to.x][to.y] = squares[from.x][from.y];
	removePiece(from);
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


bool Board::isSquareAttacked(const Position& pos, Color color) const
{
	Color opponentColor = (color == Color::White) ? Color::Black : Color::White;

	for (int x = 0; x < 8; ++x)
	{
		for (int y = 0; y < 8; ++y)
		{
			Position from(x, y);
			const Piece& piece = getPiece(from);
			if (piece.getType() != PieceType::None && piece.getColor() == opponentColor)
			{
				std::vector<Position> moves;
				switch (piece.getType())
				{
				case PieceType::Pawn:
				{
					int direction = (opponentColor == Color::White) ? 1 : -1;
					for (int dx : {-1, 1})
					{
						Position attackPos(x + dx, y + direction);
						if (attackPos.isValid() && attackPos.x == pos.x && attackPos.y == pos.y)
							return true;
					}
				}
				break;
				case PieceType::Knight:
				{
					const int kx[] = { 1, 2, 2, 1, -1, -2, -2, -1 };
					const int ky[] = { 2, 1, -1, -2, -2, -1, 1, 2 };
					for (int i = 0; i < 8; ++i)
					{
						Position attackPos(x + kx[i], y + ky[i]);
						if (attackPos.isValid() && attackPos.x == pos.x && attackPos.y == pos.y)
							return true;
					}
				}
				break;
				case PieceType::Bishop:
				case PieceType::Rook:
				case PieceType::Queen:
				{
					static const int directions[8][2] = {
						{1,0},{-1,0},{0,1},{0,-1},{1,1},{1,-1},{-1,1},{-1,-1}
					};
					int maxDir = (piece.getType() == PieceType::Bishop) ? 4 :
						(piece.getType() == PieceType::Rook) ? 4 : 8;
					int startDir = (piece.getType() == PieceType::Bishop) ? 4 :
						(piece.getType() == PieceType::Rook) ? 0 : 0;
					for (int d = startDir; d < startDir + maxDir; ++d)
					{
						int nx = x, ny = y;
						while (true)
						{
							nx += directions[d][0];
							ny += directions[d][1];
							Position attackPos(nx, ny);
							if (!attackPos.isValid()) break;
							if (attackPos.x == pos.x && attackPos.y == pos.y)
								return true;
							if (getPiece(attackPos).getType() != PieceType::None)
								break;
						}
					}
				}
				break;
				case PieceType::King:
					for (int dx = -1; dx <= 1; ++dx)
					{
						for (int dy = -1; dy <= 1; ++dy)
						{
							if (dx == 0 && dy == 0) continue;
							Position attackPos(x + dx, y + dy);
							if (attackPos.isValid() && attackPos.x == pos.x && attackPos.y == pos.y)
								return true;
						}
					}
					break;
				default:
					break;
				}
			}
		}
	}
	return false;
}

Position Board::findKingPos(Color player) const
{
	for (int x = 0; x < 8; ++x)
	{
		for (int y = 0; y < 8; ++y)
		{
			if (getPiece(Position(x, y)).getType() == PieceType::King && getPiece(Position(x, y)).getColor() == player)
			{
				return(Position(x, y));
			}
		}
	}
	return Position(-1, -1);
}
bool Board::isKingInCheck(Color player) const
{
	return isSquareAttacked(findKingPos(player), player);
}

bool Board::isInsufficientMaterial() const
{
	int whiteBishops = 0, blackBishops = 0, whiteKnights = 0, blackKnights = 0;
	int whiteOther = 0, blackOther = 0, whitePawns = 0, blackPawns = 0;
	for (int x = 0; x < 8; ++x) {
		for (int y = 0; y < 8; ++y) {
			const Piece& p = squares[x][y];
			if (p.getType() == PieceType::None || p.getType() == PieceType::King)
				continue;
			if (p.getType() == PieceType::Pawn) {
				if (p.getColor() == Color::White) whitePawns++;
				else blackPawns++;
			}
			else if (p.getType() == PieceType::Bishop) {
				if (p.getColor() == Color::White) whiteBishops++;
				else blackBishops++;
			}
			else if (p.getType() == PieceType::Knight) {
				if (p.getColor() == Color::White) whiteKnights++;
				else blackKnights++;
			}
			else {
				if (p.getColor() == Color::White) whiteOther++;
				else blackOther++;
			}
		}
	}
	// Only kings
	if (whiteBishops + blackBishops + whiteKnights + blackKnights + whiteOther + blackOther + whitePawns + blackPawns == 0)
		return true;
	// King and one knight or king and bishop
	if (whiteOther + blackOther + whitePawns + blackPawns > 0)
		return false;
	if ((whiteBishops + whiteKnights == 1 && blackBishops + blackKnights == 0) ||
		(blackBishops + blackKnights == 1 && whiteBishops + whiteKnights == 0))
		return true;
	// king and bishop vs king and bishop of the same color
	if (whiteOther + blackOther + whitePawns + blackPawns == 0 &&
		whiteBishops == 1 && blackBishops == 1) {
	
		return true;
	}
	return false;
}
