#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "BoardRenderer.h"
#include "InputHandler.h"
#include "../Board/Board.h"
#include "../Game/Game.h"
#include "../ConsoleUi/ConsoleUi.h"
#include "../ChessEngine/ChessEngine.h"

// GameSFML handles the graphical chess game using SFML
class GameSFML : public Game
{
public:
    // Constructor: initializes the window and game state
    GameSFML();

    // Main game loop
    void run();

    // Handles the player's turn and input
    void playerTurn();

    // Checks if the game has ended (checkmate, stalemate, or insufficient material)
    bool isGameCompleted();

    // Draws the color selection screen at the start of the game
    void drawColorSelection(sf::RenderWindow& window, sf::Font& font);

    // Shows the promotion dialog and returns the chosen piece type
    PieceType showPromotionDialog();

private:
    sf::RenderWindow window;                    // Main SFML window
    Board board;                                // Chess board state
    BoardRenderer renderer;                     // Responsible for drawing the board and pieces
    InputHandler inputHandler;                  // Handles mouse input and board position selection
    float squareSize = 100.0f;                  // Size of each square in pixels

    std::vector<Position> highlightedMoves;     // List of highlighted possible moves
    std::optional<Position> selectedSquare;     // Currently selected square (if any)
    ConsoleUi console;                          // Console UI for board display (optional)
    std::string endMessage;                     // Message to display when the game ends
    bool gameEnded = false;                     // True if the game has ended
    std::optional<Position> kingInCheckPos;     // Position of the king in check (if any)
    ChessEngine engine;                         // Chess engine for AI moves
    bool colorSelectionDone = false;            // True if player color has been selected
};
