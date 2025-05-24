#include "Game.h"
#include <Windows.h>
Board Game::initializeGame(const std::string& fen)
{
	Board board(fen);
	return board;
}
bool Game::isMoveLegal(const Board& board, const Position& from, const Position& to, Color player) const
{
	auto legalMoves = board.getAllLegalMoves(player);
	auto it = legalMoves.find(from);
	if (it != legalMoves.end()) {
		const auto& moves = it->second;
        if (std::find(moves.begin(), moves.end(), to) != moves.end()) {
            //simulating move on temporary board
            Board tempBoard = board;
            tempBoard.movePiece(from, to);
            if (!tempBoard.isKingInCheck(player)) {
                return true;
            }
        }
    }
	return false;
}
void Game::gameLoop(Board& board)
{
    std::cout << "Current player " << currentPlayer << "\n";
    ConsoleUi::displayBoard(board);

    if (board.isKingInCheck(currentPlayer) &&
        board.getAllLegalMoves(currentPlayer).empty()) {
        std::cout << "Check mate " << (currentPlayer == Color::White ? "Black" : "White") << " Wins\n";
        system("pause");
        exit(0); 
    }
    if (!board.isKingInCheck(currentPlayer) &&
        board.getAllLegalMoves(currentPlayer).empty()) {
        std::cout << "Stalemate! Draw.\n";
        system("pause");
        exit(0);
    }
    if (board.isInsufficientMaterial()) {
        std::cout << "Draw! Insufficient material.\n";
        system("pause");
        exit(0);
    }

    HandlePlayerInput inputHandler;
    PlayerMove move = inputHandler.getPlayerMove();
    std::vector<Position> Positions = inputHandler.translatePlayerMove(move);

    if (isMoveLegal(board, Positions[0], Positions[1], currentPlayer)) {
        board.movePiece(Positions[0], Positions[1]);
        this->currentPlayer = (this->currentPlayer == Color::White) ? Color::Black : Color::White;
    }
    else {
        std::cout << "Incorrect move try again!\n";
        system("pause");
    }

    system("cls");
}