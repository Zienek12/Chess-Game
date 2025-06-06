#include "ChessEngine.h"
#include <vector>
#include <cstdlib>
#include <ctime>
#include "../Pieces/PieceType.h"

// Makes a random legal move for the given player on the board.
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

    // Seed the random number generator
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    // Select a random move from all available moves
    int randomIndex = std::rand() % allMoves.size();
    Move randomMove = allMoves[randomIndex];

    // Make the selected move on the board
    board.movePiece(randomMove);
}

// Evaluates the board and returns a score from White's perspective.
// Positive values favor White, negative values favor Black.
float ChessEngine::evaluateBoard(const Board& board) const
{
    // Piece values: None, Pawn, Knight, Bishop, Rook, Queen, King
    const float pieceValues[] = { 0.0f, 1.0f, 3.2f, 3.3f, 5.0f, 9.0f, 0.0f };

    float score = 0.0f;
    for (int x = 0; x < 8; ++x) {
        for (int y = 0; y < 8; ++y) {
            const Piece& piece = board.squares[x][y];
            PieceType type = piece.getType();
            if (type == PieceType::None)
                continue;

            float value = pieceValues[static_cast<int>(type)];
            float bonus = 0.0f;

            // Add positional bonuses for White pieces
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
            // Subtract positional bonuses for Black pieces
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

// Minimax algorithm without alpha-beta pruning.
// Returns the evaluation score for the given board state.
float ChessEngine::minimax(Board& board, int depth, Color player)
{
    nodesVisited++;
    if (depth == 0) {
        return evaluateBoard(board);
    }
    std::map<Position, std::vector<Move>> legalMoves = board.getAllLegalMoves(player);

    // If no legal moves, check for checkmate or stalemate
    if (legalMoves.empty()) {
        if (board.isKingInCheck(player)) {
            // Checkmate
            return (player == Color::White) ? -100000.0f : 100000.0f;
        }
        else {
            // Stalemate
            return 0.0f;
        }
    }

    float bestEval = (player == Color::White) ? -100000.0f : 100000.0f;

    for (const auto& entry : legalMoves)
    {
        for (const Move& move : entry.second)
        {
            // Save the board state before making the move
            MoveState prevState = board.saveStateBeforeMove(move);

            // Make the move
            board.movePiece(move);

            // Recursively evaluate the resulting position
            float eval = minimax(board, depth - 1,
                (player == Color::White ? Color::Black : Color::White));

            // Undo the move
            board.restoreStateBeforeMove(prevState);

            // Update the best evaluation depending on the player
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

// Minimax algorithm with alpha-beta pruning.
// Returns the evaluation score for the given board state.
float ChessEngine::minimaxAlphaBeta(Board& board, int depth, Color player, float alpha, float beta)
{
    nodesVisited++;
    if (depth == 0) {
        return evaluateBoard(board);
    }
    std::map<Position, std::vector<Move>> legalMoves = board.getAllLegalMoves(player);

    // If no legal moves, check for checkmate or stalemate
    if (legalMoves.empty()) {
        if (board.isKingInCheck(player)) {
            // Checkmate
            return (player == Color::White) ? -100000.0f : 100000.0f;
        }
        else {
            // Stalemate
            return 0.0f;
        }
    }

    if (player == Color::White) {
        float maxEval = -100000.0f;
        for (const auto& entry : legalMoves) {
            for (const Move& move : entry.second) {
                // Save the board state before making the move
                MoveState prevState = board.saveStateBeforeMove(move);
                board.movePiece(move);

                // Recursively evaluate the resulting position
                float eval = minimaxAlphaBeta(board, depth - 1, Color::Black, alpha, beta);

                // Undo the move
                board.restoreStateBeforeMove(prevState);

                maxEval = std::max(maxEval, eval);
                alpha = std::max(alpha, maxEval);
                // Alpha-beta pruning: cut off search if beta <= alpha
                if (beta <= alpha)
                    return maxEval;
            }
        }
        return maxEval;
    }
    else {
        float minEval = 100000.0f;
        for (const auto& entry : legalMoves) {
            for (const Move& move : entry.second) {
                // Save the board state before making the move
                MoveState prevState = board.saveStateBeforeMove(move);
                board.movePiece(move);

                // Recursively evaluate the resulting position
                float eval = minimaxAlphaBeta(board, depth - 1, Color::White, alpha, beta);

                // Undo the move
                board.restoreStateBeforeMove(prevState);

                minEval = std::min(minEval, eval);
                beta = std::min(beta, minEval);
                // Alpha-beta pruning: cut off search if beta <= alpha
                if (beta <= alpha)
                    return minEval;
            }
        }
        return minEval;
    }
}

// Finds and returns the best move for the given player using minimax with alpha-beta pruning.
Move ChessEngine::findBestMove(Board& board, int depth, Color player) {
    nodesVisited = 0;
    std::map<Position, std::vector<Move>> legalMoves = board.getAllLegalMoves(player);
    if (legalMoves.empty()) {
        return Move(); // No available moves: draw or checkmate
    }
    Move bestMove;
    float bestEval = (player == Color::White) ? -100000.0f : 100000.0f;

    for (const auto& entry : legalMoves) {
        for (const Move& move : entry.second) {
            // Save the board state before making the move
            MoveState prevState = board.saveStateBeforeMove(move);
            // Make the move
            board.movePiece(move);

            // Evaluate the move using minimax with alpha-beta pruning
            float eval = minimax(
                board,
                depth - 1,
                (player == Color::White ? Color::Black : Color::White));

            // Undo the move
            board.restoreStateBeforeMove(prevState);

            // Update the best move if this move is better
            if ((player == Color::White && eval > bestEval) ||
                (player == Color::Black && eval < bestEval)) {
                bestEval = eval;
                bestMove = move;
            }
        }
    }
    return bestMove;
}




Move ChessEngine::findBestMoveAlphaBeta(Board& board, int depth, Color player) {
    nodesVisited = 0;
    std::map<Position, std::vector<Move>> legalMoves = board.getAllLegalMoves(player);
    if (legalMoves.empty()) {
        return Move(); // No available moves: draw or checkmate
    }
    Move bestMove;
    float bestEval = (player == Color::White) ? -100000.0f : 100000.0f;

    for (const auto& entry : legalMoves) {
        for (const Move& move : entry.second) {
            // Save the board state before making the move
            MoveState prevState = board.saveStateBeforeMove(move);
            // Make the move
            board.movePiece(move);

            // Evaluate the move using minimax with alpha-beta pruning
            float eval = minimaxAlphaBeta(
                board,
                depth - 1,
                (player == Color::White ? Color::Black : Color::White),
                -100000.0f,  // alpha
                100000.0f    // beta
            );

            // Undo the move
            board.restoreStateBeforeMove(prevState);

            // Update the best move if this move is better
            if ((player == Color::White && eval > bestEval) ||
                (player == Color::Black && eval < bestEval)) {
                bestEval = eval;
                bestMove = move;
            }
        }
    }
    return bestMove;
}
