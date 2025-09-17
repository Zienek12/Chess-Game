#include "Board/Board.h"
#include "Pieces/Piece.h"
#include <gtest/gtest.h>

class StandardBoardTestFixture : public testing :: Test{
public:
	void SetUp()
	{
	}
	void TearDown()
	{
	}
	Board board;

};

TEST_F(StandardBoardTestFixture, InitPosEQ)
{
	ASSERT_EQ(board.getPiece(Position(4, 3)).getType(), PieceType::Pawn);
	EXPECT_NE(board.getPiece(Position(4, 3)).getColor(), Color::None);
}
