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

// Default constructor: initializes the board with the standard chess starting position
Board::Board() : Board("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR") {}


// Mate in one for white player
//Board::Board() : Board("Q2bRBNN/K1kqRpp1/p1Prp3/p1pp4/6P1/5PP1/2P1Pp2/7n") {}


// Returns a reference to the piece at the given position.
// If the position is invalid, returns a static empty piece.
const Piece& Board::getPiece(const Position& pos)const {
	if (!pos.isValid())
	{
		static Piece empty;
		return empty;
	}
	return squares[pos.x][pos.y];
}

// Places a piece at the given position if the position is valid.
void Board::placePiece(const Position& pos, const Piece& piece)
{
	if (pos.isValid())
	{
		squares[pos.x][pos.y] = piece;
	}
}

// Clears the board by setting all squares to empty pieces.
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

// Constructor: initializes the board from a FEN string.
Board::Board(const std::string& fenString)
{
	initializeFromFEN(fenString);
}

// Initializes the board from a FEN string (only piece placement part).
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
			// New row
			x = 0;
			y--;
		}
		else if (isdigit(c))
		{
			// Empty squares
			x += (c - '0');
		}
		else
		{
			// Piece
			if (x >= 8 || y < 0)
			{
				throw std::invalid_argument("Incorrect FEN notation: too many characters");
			}
			placePiece(Position(x, y), charToPiece(c));
			x++;
		}
	}
}

// Converts a FEN character to a Piece object.
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

// Removes a piece from the given position (sets it to empty).
void Board::removePiece(const Position& pos)
{
	if (pos.isValid())
	{
		squares[pos.x][pos.y] = Piece(PieceType::None, Color::None);
	}
}

// Returns a map of all legal moves for the given color.
// The key is the starting position, the value is a vector of possible moves from that position.
std::map<Position, std::vector<Move>> Board::getAllLegalMoves(Color color) const
{
	std::map<Position, std::vector<Move>> moves;
	for (int x = 0; x < 8; ++x)
	{
		for (int y = 0; y < 8; ++y)
		{
			Position from(x, y);
			const Piece& piece = getPiece(from);
			if (piece.getColor() != color || piece.getType() == PieceType::None)
				continue;

			std::vector<Move> legalMoves;
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

			// Filter out moves that would leave the king in check
			std::vector<Move> trulyLegalMoves;
			for (const Move& move : legalMoves)
			{
				Board boardCopy = *this;
				boardCopy.makeMoveNoValidation(from, move.to);
				if (!boardCopy.isKingInCheck(color))
				{
					trulyLegalMoves.push_back(move);
				}
			}

			if (!trulyLegalMoves.empty())
				moves[from] = trulyLegalMoves;
		}
	}
	return moves;
}

// Makes a move on the board without any validation (used for simulation).
void Board::makeMoveNoValidation(const Position& from, const Position& to)
{
	if (!from.isValid() || !to.isValid())
		return;
	squares[to.x][to.y] = squares[from.x][from.y];
	removePiece(from);
}

// Tries to make a move if it is legal. Returns true if the move was made, false otherwise.
bool Board::movePiece(const Move& move)
{
	if (!move.from.isValid() || !move.to.isValid())
		return 0;

	const Piece& piece = getPiece(move.from);
	if (piece.getType() == PieceType::None)
		return 0;

	auto allMoves = getAllLegalMoves(piece.getColor());
	auto it = allMoves.find(move.from);
	if (it != allMoves.end())
	{
		const std::vector<Move>& legalMoves = it->second;
		// Check if the move is legal (including promotion)
		auto found = std::find_if(legalMoves.begin(), legalMoves.end(),
			[&move](const Move& m) {
				return m.to.x == move.to.x && m.to.y == move.to.y && m.promotionType == move.promotionType;
			});
		if (found != legalMoves.end())
		{
			// Castling
			if (piece.getType() == PieceType::King && std::abs(move.to.x - move.from.x) == 2)
			{
				int y = move.from.y;
				// King-side castling
				if (move.to.x > move.from.x)
				{
					squares[5][y] = squares[7][y];
					squares[7][y].setHasMoved(true);
					removePiece(Position(7, y));
				}
				// Queen-side castling
				else if (move.to.x < move.from.x)
				{
					squares[3][y] = squares[0][y];
					squares[0][y].setHasMoved(true);
					removePiece(Position(0, y));
				}
			}

			// Pawn promotion
			if (piece.getType() == PieceType::Pawn && move.promotionType != PieceType::None)
			{
				squares[move.to.x][move.to.y] = Piece(move.promotionType, piece.getColor(), true);
				removePiece(move.from);
				return true;
			}

			// Standard move
			squares[move.to.x][move.to.y] = squares[move.from.x][move.from.y];
			squares[move.to.x][move.to.y].setHasMoved(true);
			removePiece(move.from);
			return true;
		}
	}
	return false;
}

// Checks if a square is attacked by any piece of the given color's opponent.
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
					// Pawns attack diagonally
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
					// All possible knight moves
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
					// Sliding pieces: bishop, rook, queen
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
					// King attacks adjacent squares
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

// Finds the position of the king for the given player color.
// Returns std::nullopt if not found.
std::optional<Position> Board::findKingPos(Color player) const
{
	for (int x = 0; x < 8; ++x)
	{
		for (int y = 0; y < 8; ++y)
		{
			if (getPiece(Position(x, y)).getType() == PieceType::King && getPiece(Position(x, y)).getColor() == player)
			{
				return Position(x, y);
			}
		}
	}
	return std::nullopt;
}

// Checks if the king of the given color is in check.
bool Board::isKingInCheck(Color player) const
{
	return isSquareAttacked(*findKingPos(player), player);
}

// Checks if there is insufficient material to checkmate (draw by material).
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

// Saves the state of the board before a move is made (for undo functionality).
MoveState Board::saveStateBeforeMove(const Move& move) const
{
	MoveState state;
	state.from = move.from;
	state.to = move.to;
	state.movedPiece = getPiece(move.from);
	state.capturedPiece = getPiece(move.to);
	state.movedPieceHasMoved = state.movedPiece.hasBeenMoved();
	state.capturedPieceHasMoved = state.capturedPiece.hasBeenMoved();

	// Castling check
	if (state.movedPiece.getType() == PieceType::King && std::abs(move.to.x - move.from.x) == 2) {
		state.isCastling = true;
		int y = move.from.y;
		if (move.to.x > move.from.x) {
			// King side castle
			state.rookFrom = Position(7, y);
			state.rookTo = Position(5, y);
		}
		else {
			// Queen castle
			state.rookFrom = Position(0, y);
			state.rookTo = Position(3, y);
		}
		state.rookPiece = getPiece(state.rookFrom);
		state.rookHasMoved = state.rookPiece.hasBeenMoved();
	}

	// Promotion check
	if (state.movedPiece.getType() == PieceType::Pawn && move.promotionType != PieceType::None) {
		state.isPromotion = true;
		state.promotionType = move.promotionType;
	}

	return state;
}

// Restores the board to the state before a move (undoes a move).
void Board::restoreStateBeforeMove(const MoveState& state)
{
	// Undo pawn promotion
	if (state.isPromotion) {
		// Restore pawn at the source, and captured piece at the destination
		squares[state.from.x][state.from.y] = state.movedPiece;
		squares[state.from.x][state.from.y].setHasMoved(state.movedPieceHasMoved);
		squares[state.to.x][state.to.y] = state.capturedPiece;
		squares[state.to.x][state.to.y].setHasMoved(state.capturedPieceHasMoved);
		return;
	}

	// Undo castling
	if (state.isCastling) {
		// Restore king
		squares[state.from.x][state.from.y] = state.movedPiece;
		squares[state.from.x][state.from.y].setHasMoved(state.movedPieceHasMoved);
		// Restore rook
		squares[state.rookFrom.x][state.rookFrom.y] = state.rookPiece;
		squares[state.rookFrom.x][state.rookFrom.y].setHasMoved(state.rookHasMoved);
		// Remove king and rook from their destination squares
		squares[state.to.x][state.to.y] = Piece(PieceType::None, Color::None);
		squares[state.rookTo.x][state.rookTo.y] = Piece(PieceType::None, Color::None);
		return;
	}

	// Standard move (including captures)
	squares[state.from.x][state.from.y] = state.movedPiece;
	squares[state.from.x][state.from.y].setHasMoved(state.movedPieceHasMoved);
	squares[state.to.x][state.to.y] = state.capturedPiece;
	squares[state.to.x][state.to.y].setHasMoved(state.capturedPieceHasMoved);
}
