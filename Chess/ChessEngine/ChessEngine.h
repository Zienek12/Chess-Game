#include <random>
#include "../Pieces/Color.h"
#include "../Board/Board.h"

class ChessEngine
{
public:
	void makeRandomMove(Board& board, Color player);
};