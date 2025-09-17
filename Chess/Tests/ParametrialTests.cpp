#include <gtest/gtest.h>
#include "../Board/Board.h"

// Struct for test parameters
struct MovePieceParam {
    std::string fen;
    Move move;
    bool expectedResult;
};

// Parameterized test fixture
class BoardMovePieceTest : public ::testing::TestWithParam<MovePieceParam> {};

TEST_P(BoardMovePieceTest, MovePieceReturnsExpectedResult) {
    const auto& param = GetParam();
    Board board(param.fen);
    bool result = board.movePiece(param.move);
    EXPECT_EQ(result, param.expectedResult);
}

// Example test cases
INSTANTIATE_TEST_SUITE_P(
    MovePieceCases,
    BoardMovePieceTest,
    ::testing::Values(
        // e2-e4, legal move in starting position
        MovePieceParam{
            "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR",
            Move(Position(4,1), Position(4,3)),
            true
        },
        // e2-e5, illegal move in starting position
        MovePieceParam{
            "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR",
            Move(Position(4,1), Position(4,4)),
            false
        },
        // Knight move b1-c3, legal
        MovePieceParam{
            "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR",
            Move(Position(1,0), Position(2,2)),
            true
        },
        // Try to move from empty square
        MovePieceParam{
            "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR",
            Move(Position(3,3), Position(3,4)),
            false
        }
    )
);