#include "ChessEngine.h"
#include <vector>
#include <cstdlib>
#include <ctime>

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
