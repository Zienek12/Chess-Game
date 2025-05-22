#pragma once
struct Position
{
	int x, y;

	Position(int x = 0, int y = 0) : x(x), y(y) {}

	bool isValid() const
	{
		return x >= 0 && x < 8 && y >= 0 && y < 8;
	}
};
inline bool operator==(const Position& lhs, const Position& rhs)
{
	return lhs.x == rhs.x && lhs.y == rhs.y;
}