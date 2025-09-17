#include <gmock/gmock.h>
#include "../Board/Board.h"
#include "gtest/gtest.h"
class MyMock : public Board
{
public:
    MOCK_METHOD(std::optional<Position>, findKingPos, (Color player), (const));
    MOCK_METHOD(bool, isSquareAttacked, (const Position& pos, Color color), (const));

};

TEST(BoardMockTest, IsKingInCheck_CallsDependencies)
{
    MyMock mockBoard;
    Position kingPos(4, 0);

    // Oczekujemy, ?e findKingPos zostanie wywo?ane i zwróci pozycj? króla
    EXPECT_CALL(mockBoard, findKingPos(Color::White))
        .WillOnce(testing::Return(kingPos));

    // Oczekujemy, ?e isSquareAttacked zostanie wywo?ane z pozycj? króla
    EXPECT_CALL(mockBoard, isSquareAttacked(kingPos, Color::White))
        .WillOnce(testing::Return(true));

    // Wywo?anie testowanej metody
    bool result = mockBoard.isKingInCheck(Color::White);

    // Sprawdzenie wyniku (opcjonalnie, zale?nie od implementacji)
    EXPECT_TRUE(result);
}