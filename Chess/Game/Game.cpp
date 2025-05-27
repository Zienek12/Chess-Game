#include "Game.h"

Board Game::initializeGame(const std::string& fen)
{
	Board board(fen);
    ConsoleUi console;
    this->playerColor = console.askPlayerColor();
    this->aiColor = (playerColor == Color::White) ? Color::Black : Color::White;
    return board;
}
bool Game::isMoveLegal(const Board& board, const Position& from, const Position& to, Color player, PieceType promotionType) const
{
    auto legalMoves = board.getAllLegalMoves(player);
    auto it = legalMoves.find(from);
    if (it != legalMoves.end()) {
        const auto& moves = it->second;
        return std::any_of(moves.begin(), moves.end(), [&](const Move& m) {
            return m.to == to && m.promotionType == promotionType;
            });
    }
    return false;
}

bool Game::isGameCompleted(const Board& board) const
{

    if (board.isKingInCheck(currentPlayer) &&
        board.getAllLegalMoves(currentPlayer).empty()) {
        std::cout << "Check mate " << (currentPlayer == Color::White ? "Black" : "White") << " Wins\n";
        system("pause");
        return 1;
    }
    if (!board.isKingInCheck(currentPlayer) &&
        board.getAllLegalMoves(currentPlayer).empty()) {
        std::cout << "Stalemate! Draw.\n";
        system("pause");
        return 1;
    }
    if (board.isInsufficientMaterial()) {
        std::cout << "Draw! Insufficient material.\n";
        system("pause");
        return 1;
    }
    return 0;
}


void Game::gameLoop(Board& board)
{
    ConsoleUi console;
    ChessEngine engine;
    console.displayBoard(board, currentPlayer);
    if (isGameCompleted(board))
    { 
        std::cout << "\n" << counter;
        exit(0);
    }
    //Ai move
    if (currentPlayer == aiColor) {
        engine.makeRandomMove(board, aiColor);
        currentPlayer = playerColor;
        std::cout << "\n" << counter;
        counter++;

        system("cls");
        return;
    }

    //To watch how computer vs computer plays
    //Comment player move section if this is wanted
    
    if (currentPlayer == playerColor) {
        engine.makeRandomMove(board, playerColor);
        currentPlayer = aiColor;
        std::cout << "\n" << counter;
        counter++;

        system("cls");
        return;
    }
    

    /*
    //player move
    HandlePlayerInput inputHandler;
    PlayerMove move = inputHandler.getPlayerMove();
    std::vector<Position> Positions = inputHandler.translatePlayerMove(move);

    Piece movedPiece = board.getPiece(Positions[0]);
    bool isPromotion = (movedPiece.getType() == PieceType::Pawn) &&
        ((movedPiece.getColor() == Color::White && Positions[1].y == 7) ||
            (movedPiece.getColor() == Color::Black && Positions[1].y == 0));

    PieceType promotionType = PieceType::None;

    if (isPromotion) {

        promotionType = console.askPromotionChoice();
    }

    if (isMoveLegal(board, Positions[0], Positions[1], currentPlayer, promotionType)) {
        board.movePiece(Move(Positions[0], Positions[1], promotionType));
        this->currentPlayer = (this->currentPlayer == Color::White) ? Color::Black : Color::White;
    }
    else {
        std::cout << "Incorrect move try again!\n";
        system("pause");
    }
    
    system("cls");
    */

}
