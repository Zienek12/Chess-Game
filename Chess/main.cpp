#include "Board/Board.h"
#include "ConsoleUi/ConsoleUi.h"
#include "Game/Game.h"
#include <string>

int main()
{

	std::string standardBoard = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
	
	Board board = Game::initializeGame(standardBoard);

	while (1)
	{
		Game::gameLoop(board);
	}
}