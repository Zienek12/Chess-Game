#include "Game.h"

// Initializes the game board from a FEN string and asks the player to choose a color.
// Sets playerColor and aiColor accordingly.
Board Game::initializeGame(const std::string& fen)
{
    Board board(fen);
    ConsoleUi console;
    this->playerColor = console.askPlayerColor();
    this->aiColor = (playerColor == Color::White) ? Color::Black : Color::White;
    return board;
}

// Checks if a move is legal for the given player, including promotion.
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

// Checks if the game is completed (checkmate, stalemate, or insufficient material).
// Prints the result and pauses if the game is over.
bool Game::isGameCompleted(const Board& board) const
{
    if (board.isKingInCheck(currentPlayer) &&
        board.getAllLegalMoves(currentPlayer).empty()) {
        std::cout << "Checkmate! " << (currentPlayer == Color::White ? "Black" : "White") << " wins.\n";
        system("pause");
        return true;
    }
    if (!board.isKingInCheck(currentPlayer) &&
        board.getAllLegalMoves(currentPlayer).empty()) {
        std::cout << "Stalemate! Draw.\n";
        system("pause");
        return true;
    }
    if (board.isInsufficientMaterial()) {
        std::cout << "Draw! Insufficient material.\n";
        system("pause");
        return true;
    }
    return false;
}

// Main game loop: alternates between player and computer moves, displays the board, and checks for game end.
void Game::gameLoop(Board& board)
{
    ConsoleUi console;
    ChessEngine engine;

    while (true) {
        // Display the board and current player
        console.displayBoard(board, currentPlayer);

        // Check for game end
        if (isGameCompleted(board)) {
            std::cout << "\nMoves played: " << counter << "\n";
            exit(0);
        }

        // Computer (AI) move
        if (currentPlayer == aiColor) {
            Move aiMove = engine.findBestMove(board, 3, aiColor);
            board.movePiece(aiMove);

            std::cout << "\nAI move: " << char('a' + aiMove.from.x) << (aiMove.from.y + 1)
                << " -> " << char('a' + aiMove.to.x) << (aiMove.to.y + 1) << "\n";
            std::cout << "Nodes visited: " << engine.getNodesVisited() << "\n";
            std::cout << "Board evaluation: " << engine.evaluateBoard(board) << "\n";
            counter++;
            currentPlayer = playerColor;

            system("pause");
            system("cls");
            continue;
        }

        // Player move
        HandlePlayerInput inputHandler;
        PlayerMove move = inputHandler.getPlayerMove();
        std::vector<Position> positions = inputHandler.translatePlayerMove(move);

        if (positions.size() < 2) {
            std::cout << "Invalid input. Try again.\n";
            system("pause");
            system("cls");
            continue;
        }

        Piece movedPiece = board.getPiece(positions[0]);
        bool isPromotion = (movedPiece.getType() == PieceType::Pawn) &&
            ((movedPiece.getColor() == Color::White && positions[1].y == 7) ||
                (movedPiece.getColor() == Color::Black && positions[1].y == 0));

        PieceType promotionType = PieceType::None;
        if (isPromotion) {
            promotionType = console.askPromotionChoice();
        }

        if (isMoveLegal(board, positions[0], positions[1], currentPlayer, promotionType)) {
            board.movePiece(Move(positions[0], positions[1], promotionType));
            currentPlayer = aiColor;
            counter++;
        }
        else {
            std::cout << "Incorrect move, try again!\n";
            system("pause");
        }

        system("cls");
    }
}
