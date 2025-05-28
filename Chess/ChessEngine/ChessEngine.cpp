#include "ChessEngine.h"
#include <vector>
#include <cstdlib>
#include <ctime>
#include "../Pieces/PieceType.h"
void ChessEngine::makeRandomMove(Board& board, Color player) {
    // Get all legal moves for the given player
    std::map<Position, std::vector<Move>> legalMoves = board.getAllLegalMoves(player);

    // If there are no legal moves, return (e.g., checkmate or stalemate)
    if (legalMoves.empty()) {
        return;
    }

    // Flatten the map of moves into a single vector
    std::vector<Move> allMoves;
    for (const auto& entry : legalMoves) {
        allMoves.insert(allMoves.end(), entry.second.begin(), entry.second.end());
    }

    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    //Select random move
    int randomIndex = std::rand() % allMoves.size();
    Move randomMove = allMoves[randomIndex];

    board.movePiece(randomMove);
}

float ChessEngine::evaluateBoard(const Board& board) const
{
    const float pieceValues[] = { 0.0f, 1.0f, 3.2f, 3.3f, 5.0f, 9.0f, 0.0f }; // None, Pawn, Knight, Bishop, Rook, Queen, King

    float score = 0.0f;
    for (int x = 0; x < 8; ++x) {
        for (int y = 0; y < 8; ++y) {
            const Piece& piece = board.squares[x][y];
            PieceType type = piece.getType();
            if (type == PieceType::None)
                continue;

            float value = pieceValues[static_cast<int>(type)];
            float bonus = 0.0f;

            if (piece.getColor() == Color::White) {
                switch (type) {
                case PieceType::Pawn:   bonus = WHITE_PAWN_PREFERRED_COORDINATES[x][y]; break;
                case PieceType::Knight: bonus = WHITE_KNIGHT_PREFERRED_COORDINATES[x][y]; break;
                case PieceType::Bishop: bonus = WHITE_BISHOP_PREFERRED_COORDINATES[x][y]; break;
                case PieceType::Rook:   bonus = WHITE_ROOK_PREFERRED_COORDINATES[x][y]; break;
                case PieceType::Queen:  bonus = WHITE_QUEEN_PREFERRED_COORDINATES[x][y]; break;
                case PieceType::King:   bonus = WHITE_KING_PREFERRED_COORDINATES[x][y]; break;
                default: break;
                }
                score += value + bonus;
            }
            else if (piece.getColor() == Color::Black) {
                switch (type) {
                case PieceType::Pawn:   bonus = BLACK_PAWN_PREFERRED_COORDINATES[x][y]; break;
                case PieceType::Knight: bonus = BLACK_KNIGHT_PREFERRED_COORDINATES[x][y]; break;
                case PieceType::Bishop: bonus = BLACK_BISHOP_PREFERRED_COORDINATES[x][y]; break;
                case PieceType::Rook:   bonus = BLACK_ROOK_PREFERRED_COORDINATES[x][y]; break;
                case PieceType::Queen:  bonus = BLACK_QUEEN_PREFERRED_COORDINATES[x][y]; break;
                case PieceType::King:   bonus = BLACK_KING_PREFERRED_COORDINATES[x][y]; break;
                default: break;
                }
                score -= value + bonus;
            }
        }
    }
    return score;
}

float ChessEngine::minimax(Board& board, int depth, Color player)
{
    if (depth == 0) {
        return evaluateBoard(board);
    }
    std::map<Position, std::vector<Move>> legalMoves = board.getAllLegalMoves(player);

    if (legalMoves.empty()) {
        if (board.isKingInCheck(player)) {
            // Mate
            return (player == Color::White) ? -100000.0f : 100000.0f;
        }
        else {
            // stalemate
            return 0.0f;
        }
    }

    float bestEval = (player == Color::White) ? -100000.0f : 100000.0f;

    for (const auto& entry : legalMoves)
    {
        for (const Move& move : entry.second)
        {
            // Zapisz stan przed ruchem
            MoveState prevState = board.saveStateBeforeMove(move);

            // Wykonaj ruch
            board.movePiece(move);

            float eval = minimax(board, depth - 1,
                (player == Color::White ? Color::Black : Color::White));

            // Cofnij ruch
            board.restoreStateBeforeMove(prevState);

            if (player == Color::White)
            {
                if (eval > bestEval) bestEval = eval;
            }
            else
            {
                if (eval < bestEval) bestEval = eval;
            }
        }
    }
    return bestEval;
}

Move ChessEngine::findBestMove(Board& board, int depth, Color player) {
    std::map<Position, std::vector<Move>> legalMoves = board.getAllLegalMoves(player);
    if (legalMoves.empty()) {
        return Move(); // No available moves draw or checkmate
    }
    Move bestMove;
    float bestEval = (player == Color::White) ? -100000.0f : 100000.0f;

    for (const auto& entry : legalMoves) {
        for (const Move& move : entry.second) {
            // Save state before move
            MoveState prevState = board.saveStateBeforeMove(move);
            // Make move
        
            board.movePiece(move);

            // Call minimax for new position
            float eval = minimax(board, depth - 1, (player == Color::White ? Color::Black : Color::White));

            //Undo move
            board.restoreStateBeforeMove(prevState);

            // Update best move
            if ((player == Color::White && eval > bestEval) ||
                (player == Color::Black && eval < bestEval)) {
                bestEval = eval;
                bestMove = move;
            }
        }
    }
    return bestMove;
}

