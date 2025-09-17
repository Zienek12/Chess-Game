#pragma once

#include "../Pieces/Piece.h"
#include "../Pieces/Position.h"
#include "../Pieces/Move.h"
#include "MoveState.h"
#include <array>
#include <string>
#include <stdexcept>
#include <map>
#include <optional>

// The Board class represents the chessboard and provides methods for manipulating pieces and game state.
class Board
{
private:
    // Makes a move on the board without validation (used for simulation).
    void makeMoveNoValidation(const Position& from, const Position& to);

protected:
    // Initializes the board from a FEN string (piece placement only).
    void initializeFromFEN(const std::string& fenString);
    // Converts a FEN character to a Piece object.
    Piece charToPiece(char c) const;

public:
    // 8x8 array representing the chessboard squares.
    std::array<std::array<Piece, 8>, 8> squares;

    // Constructors
    Board();
    Board(const std::string& fenString);

    // Returns a const reference to the piece at the given position.
    const Piece& getPiece(const Position& pos) const;
    // Places a piece at the given position.
    void placePiece(const Position& pos, const Piece& piece);
    // Clears the board (sets all squares to empty).
    void clearBoard();
    // Removes a piece from the given position.
    void removePiece(const Position& pos);
    // Tries to make a move if it is legal. Returns true if the move was made.
    bool movePiece(const Move& move);
    // Returns a map of all legal moves for the given color.
    std::map<Position, std::vector<Move>> getAllLegalMoves(Color color) const;
    // Checks if a square is attacked by the opponent.
    virtual bool isSquareAttacked(const Position& pos, Color color) const;

    // Finds the position of the king for the given player color.
    virtual std::optional<Position> findKingPos(Color player) const;
    // Checks if the king of the given color is in check.
    bool isKingInCheck(Color color) const;

    // Checks if there is insufficient material to checkmate.
    bool isInsufficientMaterial() const;

    // Saves the state of the board before a move is made (for undo functionality).
    MoveState saveStateBeforeMove(const Move& move) const;
    // Restores the board to the state before a move (undoes a move).
    void restoreStateBeforeMove(const MoveState& state);

    virtual ~Board() = default;
};
