#include "GameSFML.h"
#include <optional>
#include "InputHandler.h"
#include "../Pieces/Move.h"

// Constructor: Initializes the SFML window and sets the square size
GameSFML::GameSFML()
    :
    window(sf::VideoMode({ 800, 800 }), "Chess"),
    squareSize(100.0f)
{
    window.setFramerateLimit(60);
}

// Checks if the game has ended (checkmate, stalemate, or insufficient material)
bool GameSFML::isGameCompleted() {
    // Checkmate condition
    if (board.isKingInCheck(currentPlayer) &&
        board.getAllLegalMoves(currentPlayer).empty()) {
        endMessage = "Check mate! " + std::string(currentPlayer == Color::White ? "Black" : "White") + " wins";
        gameEnded = true;
        return true;
    }
    // Stalemate condition
    if (!board.isKingInCheck(currentPlayer) &&
        board.getAllLegalMoves(currentPlayer).empty()) {
        endMessage = "Stalemate! Draw.";
        gameEnded = true;
        return true;
    }
    // Insufficient material condition
    if (board.isInsufficientMaterial()) {
        endMessage = "Draw! Insufficient material.";
        gameEnded = true;
        return true;
    }
    return false;
}

void GameSFML::playerTurn()
{
    // Handle window and input events
    while (const std::optional event = window.pollEvent()) {
        // Handle window close event
        if (event->is<sf::Event::Closed>()) {
            window.close();
            continue;
        }

        // Ignore input if game ended or event is not mouse button press
        if (gameEnded || !event->is<sf::Event::MouseButtonPressed>()) {
            continue;
        }

        // Handle mouse click and get board position
        auto pos = inputHandler.handleEvent(*event, window);
        if (!pos) continue;

        const auto piece = board.getPiece(*pos);

        // Select or change selected square
        if (!selectedSquare) {
            if (piece.getColor() == currentPlayer) {
                selectedSquare = pos;
                highlightedMoves.clear();
                const auto allLegalMoves = board.getAllLegalMoves(currentPlayer);
                auto it = allLegalMoves.find(*selectedSquare);
                if (it != allLegalMoves.end()) {
                    for (const auto& move : it->second) {
                        highlightedMoves.push_back(move.to);
                    }
                }
            }
            continue;
        }

        // Deselect if the same square is clicked
        if (*selectedSquare == *pos) {
            selectedSquare.reset();
            highlightedMoves.clear();
            continue;
        }

        // Change selection if another piece of the current player is clicked
        if (piece.getColor() == currentPlayer) {
            selectedSquare = pos;
            highlightedMoves.clear();
            const auto allLegalMoves = board.getAllLegalMoves(currentPlayer);
            auto it = allLegalMoves.find(*selectedSquare);
            if (it != allLegalMoves.end()) {
                for (const auto& move : it->second) {
                    highlightedMoves.push_back(move.to);
                }
            }
            continue;
        }

        // Attempt to make a move
        Move move(*selectedSquare, *pos);
        if (board.movePiece(move)) {
            system("cls");
            selectedSquare.reset();
            highlightedMoves.clear();
            currentPlayer = (currentPlayer == Color::Black) ? Color::White : Color::Black;
            console.displayBoard(board, currentPlayer);
        }
        else {
            selectedSquare.reset();
            highlightedMoves.clear();
        }

        if (board.isKingInCheck(currentPlayer)) {
            kingInCheckPos = board.findKingPos(currentPlayer);
        }
        else
        {
            kingInCheckPos.reset();
        }

    }
}


// Main game loop
void GameSFML::run() {
    console.displayBoard(board, currentPlayer);
    sf::Font font;
    // Load font for displaying end game messages
    if (!font.openFromFile("../../../../Chess/Assets/arial.ttf")) {
        std::cerr << "Failed to load font: arial.ttf" << std::endl;
        return;
    }

    while (window.isOpen()) {
        // Check if the game has ended
        if (!gameEnded) {
            isGameCompleted();
        }

        if (currentPlayer == playerColor)
        {
            playerTurn();
           
        }
        else
        {
            board.movePiece(engine.findBestMoveAlphaBeta(board, 3, aiColor));
            currentPlayer = (currentPlayer == Color::Black) ? Color::White : Color::Black;
            std::cout << "\n\nOdwiedzone wezly: " << engine.getNodesVisited() << std::endl;

        }
        // Draw the chessboard and pieces
       
        window.clear();
        renderer.draw(window, board, selectedSquare, highlightedMoves, kingInCheckPos);
        // Draw end game message if the game has ended
        if (gameEnded) {
            sf::Text text(font);
            text.setString(endMessage);
            text.setCharacterSize(48);
            text.setFillColor(sf::Color::Red);
            text.setStyle(sf::Text::Bold);

            sf::FloatRect textRect = text.getLocalBounds();
            text.setOrigin(
                textRect.getCenter()
            );

            text.setPosition(sf::Vector2f(window.getSize().x / 2.0f, window.getSize().y / 2.0f));

            window.draw(text);
        }

        window.display();
    }
}
