#include "../Board/Board.h"
#include "../Pieces/Piece.h"
#include <gtest/gtest.h>
#include <random>
#include "../ConsoleUi/ConsoleUi.h"
class StandardBoardTestFixture : public testing :: Test{
public:
	void SetUp()
	{
	}
	void TearDown()
	{
	}
	Board board;
	void AssertInitBoardSetup(const Board&)
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
};

TEST_F(StandardBoardTestFixture, InitialBoardSetup)
{
	AssertInitBoardSetup(board);
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
	Position pawnPos(1, 1);
	ASSERT_TRUE(moves.find(pawnPos) != moves.end());
	EXPECT_EQ(moves[pawnPos].size(), 2);
}

TEST_F(StandardBoardTestFixture, PawnPromotion)
{
    board.clearBoard();
    Piece whitePawn(PieceType::Pawn, Color::White);
    Position from(0, 6);
    Position to(0, 7);
    board.placePiece(from, whitePawn);

	board.placePiece(Position(0, 0), Piece(PieceType::King, Color::White));
	board.placePiece(Position(7, 7), Piece(PieceType::King, Color::Black));


    Move promotionMove(from, to, PieceType::Queen);
    ASSERT_TRUE(board.movePiece(promotionMove));
    EXPECT_EQ(board.getPiece(to).getType(), PieceType::Queen);
    EXPECT_EQ(board.getPiece(to).getColor(), Color::White);
}

TEST_F(StandardBoardTestFixture, SaveAndRestoreBoardStateWithRandomMoves)
{
	std::vector<MoveState> savedStates;
	ConsoleUi ui;
	int movesToMake = 15;
	Color currentColor = Color::White;
//	ui.displayBoard(board, currentColor);


	for (int i = 0; i < movesToMake; ++i)
	{
		auto legalMoves = board.getAllLegalMoves(currentColor);
		if (legalMoves.empty())
			break;

		std::vector<Move> allMoves;
		for (const auto& entry : legalMoves)
		{
			for (const auto& move : entry.second)
				allMoves.push_back(move);
		}
		if (allMoves.empty())
			break;

		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<> dis(0, static_cast<int>(allMoves.size()) - 1);
		const Move& randomMove = allMoves[dis(gen)];

		savedStates.push_back(board.saveStateBeforeMove(randomMove));

		board.movePiece(randomMove);
		//ui.displayBoard(board, currentColor);

		currentColor = (currentColor == Color::White) ? Color::Black : Color::White;
	}

	for (auto it = savedStates.rbegin(); it != savedStates.rend(); ++it)
	{
		board.restoreStateBeforeMove(*it);
	}
	//ui.displayBoard(board, currentColor);

	AssertInitBoardSetup(board);
}