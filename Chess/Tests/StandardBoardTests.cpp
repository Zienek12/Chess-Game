#include "../Board/Board.h"
#include "../Pieces/Piece.h"
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
TEST_F(StandardBoardTestFixture, InitialBoardSetup)
{
	//WHITE PIECES
	EXPECT_EQ(board.getPiece(Position(0, 0)).getType(), PieceType::Rook);
	EXPECT_EQ(board.getPiece(Position(1, 0)).getType(), PieceType::Knight);
	EXPECT_EQ(board.getPiece(Position(2, 0)).getType(), PieceType::Bishop);
	EXPECT_EQ(board.getPiece(Position(3, 0)).getType(), PieceType::Queen);
	EXPECT_EQ(board.getPiece(Position(4, 0)).getType(), PieceType::King);
	EXPECT_EQ(board.getPiece(Position(5, 0)).getType(), PieceType::Bishop);
	EXPECT_EQ(board.getPiece(Position(6, 0)).getType(), PieceType::Knight);
	EXPECT_EQ(board.getPiece(Position(7, 0)).getType(), PieceType::Rook);
	for (int i = 0; i <= 7; i++)
	{
		EXPECT_EQ(board.getPiece(Position(i, 1)).getType(), PieceType::Pawn);

	}
	for (int x = 0; x <= 7; x++)
	{
		for (int y = 0; y < 2; y++)
		{
			EXPECT_EQ(board.getPiece(Position(x, y)).getColor(), Color::White);

		}
	}
	//BLACK PIECES
	EXPECT_EQ(board.getPiece(Position(0, 7)).getType(), PieceType::Rook);
	EXPECT_EQ(board.getPiece(Position(1, 7)).getType(), PieceType::Knight);
	EXPECT_EQ(board.getPiece(Position(2, 7)).getType(), PieceType::Bishop);
	EXPECT_EQ(board.getPiece(Position(3, 7)).getType(), PieceType::Queen);
	EXPECT_EQ(board.getPiece(Position(4, 7)).getType(), PieceType::King);
	EXPECT_EQ(board.getPiece(Position(5, 7)).getType(), PieceType::Bishop);
	EXPECT_EQ(board.getPiece(Position(6, 7)).getType(), PieceType::Knight);
	EXPECT_EQ(board.getPiece(Position(7, 7)).getType(), PieceType::Rook);
	for (int i = 0; i <= 7; i++)
	{
		EXPECT_EQ(board.getPiece(Position(i, 6)).getType(), PieceType::Pawn);
	}

	for (int x = 0; x <= 7; x++)
	{
		for (int y = 6; y <= 7; y++)
		{
			EXPECT_EQ(board.getPiece(Position(x, y)).getColor(), Color::Black);

		}
	}
	//EMPTY SQUARES
	for (int i = 0; i <= 7; i++)
	{
		for (int j = 2; j < 6; j++)
		{
			EXPECT_EQ(board.getPiece(Position(i, j)).getType(), PieceType::None);
		}
	}
	for (int x = 0; x <= 7; x++)
	{
		for (int y = 2; y < 6; y++)
		{
			EXPECT_EQ(board.getPiece(Position(x, y)).getColor(), Color::None);

		}
	}
}

TEST_F(StandardBoardTestFixture, ClearBoardTest)
{
	board.clearBoard();
	for (int x = 0; x < 8; x++)
	{
		for (int y = 0; y < 8; y++)
		{
			EXPECT_EQ(board.getPiece(Position(x, y)).getColor(), Color::None);
		}
	}
}

TEST_F(StandardBoardTestFixture, PlaceAndRemovePiece)
{
	Piece queen(PieceType::Queen, Color::White);
	Position pos(3, 3);
	board.placePiece(pos, queen);
	EXPECT_EQ(board.getPiece(pos).getType(), PieceType::Queen);

	board.removePiece(pos);
	EXPECT_EQ(board.getPiece(pos).getType(), PieceType::None);
}

TEST_F(StandardBoardTestFixture, PawnInitMoves)
{
	auto moves = board.getAllLegalMoves(Color::White);
	Position pawnPos(1, 0);
	ASSERT_TRUE(moves.find(pawnPos) != moves.end());
	EXPECT_EQ(moves[pawnPos].size(), 2);
}