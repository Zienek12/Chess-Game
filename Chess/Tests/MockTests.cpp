#include <gmock/gmock.h>
#include "../Board/Board.h"
#include "gtest/gtest.h"
class MyMock : public Board
{
public:
    MOCK_METHOD(std::optional<Position>, findKingPos, (Color player), (const));
    MOCK_METHOD(bool, isSquareAttacked, (const Position& pos, Color color), (const));

};

TEST(BoardMockTest, IsKingInCheck)
{
    MyMock mockBoard;
    Position kingPos(4, 0);

    EXPECT_CALL(mockBoard, findKingPos(Color::White))
        .WillOnce(testing::Return(kingPos));

    EXPECT_CALL(mockBoard, isSquareAttacked(kingPos, Color::White))
        .WillOnce(testing::Return(true));

    bool result = mockBoard.isKingInCheck(Color::White);

    EXPECT_TRUE(result);
}