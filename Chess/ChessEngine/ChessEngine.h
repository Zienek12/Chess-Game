#include <random>
#include "../Pieces/Color.h"
#include "../Board/Board.h"
#include "../Pieces/Move.h"
#include "../Board/MoveState.h"
class ChessEngine
{
public:
	void makeRandomMove(Board& board, Color player);
	float evaluateBoard(const Board& board) const;
    float minimax(Board& board, int depth, Color player);
    Move findBestMove(Board& board, int depth, Color player);
    Move findBestMoveAlphaBeta(Board& board, int depth, Color player);
    float minimaxAlphaBeta(Board& board, int depth, Color player, float alpha, float beta);
    size_t getNodesVisited() const { return nodesVisited; }

protected:
    size_t nodesVisited = 0;

private:

    const float WHITE_PAWN_PREFERRED_COORDINATES[8][8] =
    {
            0.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.0f,
            0.9f, 0.9f, 0.9f, 0.9f, 0.9f, 0.9f, 0.9f, 0.9f,
            0.3f, 0.3f, 0.4f, 0.6f, 0.6f, 0.4f, 0.3f, 0.3f,
            0.1f, 0.1f, 0.2f, 0.4f, 0.4f, 0.2f, 0.1f, 0.1f,
            0.05f, 0.05f, 0.1f, 0.2f, 0.2f, 0.1f, 0.05f, 0.05f,
            0.0f,  0.0f,  0.0f, -0.1f, -0.1f, 0.0f,  0.0f,  0.0f,
            0.05f, -0.05f, -0.1f, 0.0f, 0.0f, -0.1f, -0.05f, 0.05f,
            0.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.0f
    };
    const float BLACK_PAWN_PREFERRED_COORDINATES[8][8] =
    {
            0.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.0f,
            0.05f, -0.05f, -0.1f, 0.0f, 0.0f, -0.1f, -0.05f, 0.05f,
            0.0f,  0.0f,  0.0f, -0.1f, -0.1f, 0.0f,  0.0f,  0.0f,
            0.05f, 0.05f, 0.1f, 0.2f, 0.2f, 0.1f, 0.05f, 0.05f,
            0.1f, 0.1f, 0.2f, 0.4f, 0.4f, 0.2f, 0.1f, 0.1f,
            0.3f, 0.3f, 0.4f, 0.6f, 0.6f, 0.4f, 0.3f, 0.3f,
            0.9f, 0.9f, 0.9f, 0.9f, 0.9f, 0.9f, 0.9f, 0.9f,
            0.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.0f
    };

    const float WHITE_KNIGHT_PREFERRED_COORDINATES[8][8] =
    {
            -0.5f, -0.4f, -0.3f, -0.3f, -0.3f, -0.3f, -0.4f, -0.5f,
            -0.4f, -0.2f,  0.0f,  0.05f,  0.05f,  0.0f, -0.2f, -0.4f,
            -0.3f,  0.05f, 0.1f,  0.15f, 0.15f, 0.1f,  0.05f, -0.3f,
            -0.3f,  0.05f, 0.15f, 0.2f,  0.2f,  0.15f, 0.05f, -0.3f,
            -0.3f,  0.05f, 0.15f, 0.2f,  0.2f,  0.15f, 0.05f, -0.3f,
            -0.3f,  0.05f, 0.1f,  0.15f, 0.15f, 0.1f,  0.05f, -0.3f,
            -0.4f, -0.2f,  0.0f,  0.0f,  0.0f,  0.0f, -0.2f, -0.4f,
            -0.5f, -0.4f, -0.3f, -0.3f, -0.3f, -0.3f, -0.4f, -0.5f
    };

    const float BLACK_KNIGHT_PREFERRED_COORDINATES[8][8] = {
            -0.5f, -0.4f, -0.3f, -0.3f, -0.3f, -0.3f, -0.4f, -0.5f,
            -0.4f, -0.2f,  0.0f,  0.0f,  0.0f,  0.0f, -0.2f, -0.4f,
            -0.3f,  0.05f, 0.1f,  0.15f, 0.15f, 0.1f,  0.05f, -0.3f,
            -0.3f,  0.05f, 0.15f, 0.2f,  0.2f,  0.15f, 0.05f, -0.3f,
            -0.3f,  0.05f, 0.15f, 0.2f,  0.2f,  0.15f, 0.05f, -0.3f,
            -0.3f,  0.05f, 0.1f,  0.15f, 0.15f, 0.1f,  0.05f, -0.3f,
            -0.4f, -0.2f,  0.0f,  0.05f, 0.05f, 0.0f, -0.2f, -0.4f,
            -0.5f, -0.4f, -0.3f, -0.3f, -0.3f, -0.3f, -0.4f, -0.5f,
    };

    const float WHITE_BISHOP_PREFERRED_COORDINATES[8][8] = {
            -0.2f, -0.1f, -0.1f, -0.1f, -0.1f, -0.1f, -0.1f, -0.2f,
            -0.1f,  0.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.0f, -0.1f,
            -0.1f,  0.0f,  0.05f, 0.1f,  0.1f,  0.05f, 0.0f, -0.1f,
            -0.1f,  0.05f, 0.05f, 0.1f,  0.1f,  0.05f, 0.05f, -0.1f,
            -0.1f,  0.0f,  0.1f,  0.15f, 0.15f, 0.1f,  0.0f, -0.1f,
            -0.1f,  0.1f,  0.1f,  0.1f,  0.1f,  0.1f,  0.1f, -0.1f,
            -0.1f,  0.05f, 0.0f,  0.0f,  0.0f,  0.0f,  0.05f, -0.1f,
            -0.2f, -0.1f, -0.1f, -0.1f, -0.1f, -0.1f, -0.1f, -0.2f
    };
    const float BLACK_BISHOP_PREFERRED_COORDINATES[8][8] = {
           -0.2f, -0.1f, -0.1f, -0.1f, -0.1f, -0.1f, -0.1f, -0.2f,
           -0.1f,  0.05f, 0.0f,  0.0f,  0.0f,  0.0f,  0.05f, -0.1f,
           -0.1f,  0.1f,  0.1f,  0.1f,  0.1f,  0.1f,  0.1f, -0.1f,
           -0.1f,  0.0f,  0.1f,  0.15f, 0.15f, 0.1f,  0.0f, -0.1f,
           -0.1f,  0.05f, 0.1f,  0.15f, 0.15f, 0.1f,  0.05f, -0.1f,
           -0.1f,  0.0f,  0.1f,  0.1f,  0.1f,  0.1f,  0.0f, -0.1f,
           -0.1f,  0.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.0f, -0.1f,
           -0.2f, -0.1f, -0.1f, -0.1f, -0.1f, -0.1f, -0.1f, -0.2f
    };

    const float WHITE_ROOK_PREFERRED_COORDINATES[8][8] = {
            0.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.0f,
            0.05f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.05f,
            -0.05f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, -0.05f,
            -0.05f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, -0.05f,
            -0.05f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, -0.05f,
            -0.05f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, -0.05f,
            -0.05f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, -0.05f,
            0.0f,  0.0f,  0.0f,  0.05f, 0.05f, 0.0f,  0.0f,  0.0f
    };

    const float BLACK_ROOK_PREFERRED_COORDINATES[8][8] = {
            0.0f,  0.0f,  0.0f,  0.05f, 0.05f, 0.0f,  0.0f,  0.0f,
            -0.05f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, -0.05f,
            -0.05f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, -0.05f,
            -0.05f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, -0.05f,
            -0.05f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, -0.05f,
            -0.05f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, -0.05f,
            0.05f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.05f,
            0.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.0f,
    };

    const float WHITE_QUEEN_PREFERRED_COORDINATES[8][8] = {
            -0.2f, -0.1f, -0.1f, -0.05f, -0.05f, -0.1f, -0.1f, -0.2f,
            -0.1f,  0.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.0f, -0.1f,
            -0.1f,  0.0f,  0.05f, 0.05f, 0.05f, 0.05f, 0.0f, -0.1f,
            -0.05f, 0.0f, 0.05f, 0.1f,  0.1f,  0.05f, 0.0f, -0.05f,
            -0.05f, 0.0f, 0.05f, 0.1f,  0.1f,  0.05f, 0.0f, -0.05f,
            -0.1f,  0.0f,  0.05f, 0.05f, 0.05f, 0.05f, 0.0f, -0.1f,
            -0.1f,  0.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.0f, -0.1f,
            -0.2f, -0.1f, -0.1f, -0.05f, -0.05f, -0.1f, -0.1f, -0.2f
    };

    const float BLACK_QUEEN_PREFERRED_COORDINATES[8][8] = {
            -0.2f, -0.1f, -0.1f, -0.05f, -0.05f, -0.1f, -0.1f, -0.2f,
            -0.1f,  0.0f,  0.05f, 0.0f,  0.0f,  0.0f,  0.0f, -0.1f,
            -0.1f,  0.05f, 0.05f, 0.05f, 0.05f, 0.05f, 0.0f, -0.1f,
            -0.05f, 0.0f, 0.05f, 0.1f,  0.1f,  0.05f, 0.0f, -0.05f,
            -0.05f, 0.0f, 0.05f, 0.1f,  0.1f,  0.05f, 0.0f, -0.05f,
            -0.1f,  0.0f,  0.05f, 0.05f, 0.05f, 0.05f, 0.0f, -0.1f,
            -0.1f,  0.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.0f, -0.1f,
            -0.2f, -0.1f, -0.1f, -0.05f, -0.05f, -0.1f, -0.1f, -0.2f
    };

    const float WHITE_KING_PREFERRED_COORDINATES[8][8] = {
            -0.5f, -0.3f, -0.3f, -0.3f, -0.3f, -0.3f, -0.3f, -0.5f,
            -0.3f, -0.3f,  0.0f,  0.0f,  0.0f,  0.0f, -0.3f, -0.3f,
            -0.3f, -0.1f,  0.2f,  0.3f,  0.3f,  0.2f, -0.1f, -0.3f,
            -0.3f, -0.1f,  0.3f,  0.4f,  0.4f,  0.3f, -0.1f, -0.3f,
            -0.3f, -0.1f,  0.3f,  0.4f,  0.4f,  0.3f, -0.1f, -0.3f,
            -0.3f, -0.1f,  0.2f,  0.3f,  0.3f,  0.2f, -0.1f, -0.3f,
            -0.3f, -0.2f, -0.1f,  0.0f,  0.0f, -0.1f, -0.2f, -0.3f,
            -0.5f, -0.4f, -0.3f, -0.2f, -0.2f, -0.3f, -0.4f, -0.5f
    };

    const float BLACK_KING_PREFERRED_COORDINATES[8][8] = {
            -0.5f, -0.4f, -0.3f, -0.2f, -0.2f, -0.3f, -0.4f, -0.5f,
            -0.3f, -0.2f, -0.1f,  0.0f,  0.0f, -0.1f, -0.2f, -0.3f,
            -0.3f, -0.1f,  0.2f,  0.3f,  0.3f,  0.2f, -0.1f, -0.3f,
            -0.3f, -0.1f,  0.3f,  0.4f,  0.4f,  0.3f, -0.1f, -0.3f,
            -0.3f, -0.1f,  0.3f,  0.4f,  0.4f,  0.3f, -0.1f, -0.3f,
            -0.3f, -0.1f,  0.2f,  0.3f,  0.3f,  0.2f, -0.1f, -0.3f,
            -0.3f, -0.3f,  0.0f,  0.0f,  0.0f,  0.0f, -0.3f, -0.3f,
            -0.5f, -0.3f, -0.3f, -0.3f, -0.3f, -0.3f, -0.3f, -0.5f
    };


};