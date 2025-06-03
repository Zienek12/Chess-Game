#include "GameSFML.h"
#include <optional>
#include "InputHandler.h"
#include "../Pieces/Move.h"

GameSFML::GameSFML()
    :
    window(sf::VideoMode({ 800, 800 }), "Chess"),
    squareSize(100.0f)
{}

void GameSFML::run() {

    while (window.isOpen()) {

        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
            if (!event->is<sf::Event::MouseButtonPressed>())
                continue;

            auto pos = inputHandler.handleEvent(*event, window);
            if (!pos)
                continue;

            const auto piece = board.getPiece(*pos);

            // If no square is currently selected
            // Select the square if it contains the current player's piece
            if (!selectedSquare) {
                if (piece.getColor() == currentPlayer)
                {
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

            // If the same square is clicked again – deselect it
            // Allows the player to cancel their selection
            if (*selectedSquare == *pos) {
                selectedSquare.reset();
                continue;
            }

            // If a piece of the same color is clicked – change selection
            // Useful for quickly switching the selected piece before making a move
            if (piece.getColor() == currentPlayer) {
                selectedSquare = pos;
                highlightedMoves.clear();
                const auto allLegalMoves = board.getAllLegalMoves(currentPlayer);
                for (const auto& [pos, moves] : allLegalMoves) {
                    if (!moves.empty()) {
                        highlightedMoves.push_back(pos);
                    }
                }
                continue;
            }
            Move move(*selectedSquare, *pos);
            board.movePiece(move);
            selectedSquare.reset();
            highlightedMoves.clear();
            currentPlayer = (currentPlayer == Color::Black) ? Color::White : Color::Black;

        }
        window.clear();
        renderer.draw(window, board, selectedSquare, highlightedMoves);
        window.display();
    }
}
