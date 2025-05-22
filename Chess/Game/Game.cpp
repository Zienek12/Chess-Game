#include "Game.h"

Board Game::initializeGame(const std::string& fen)
{
	Board board(fen);
	return board;
}
void Game::gameLoop(Board& board)
{
	ConsoleUi::displayBoard(board);

	HandlePlayerInput inputHandler;
	PlayerMove move = inputHandler.getPlayerMove();
	std::vector<Position> Positions;
	Positions = inputHandler.translatePlayerMove(move);

	board.movePiece(Positions[0], Positions[1]);
	

}