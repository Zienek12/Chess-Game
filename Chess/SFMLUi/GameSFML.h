#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "BoardRenderer.h"
#include "InputHandler.h"
#include "../Board/Board.h"
#include "../Game/Game.h"
#include "../ConsoleUi/ConsoleUi.h"
#include "../ChessEngine/ChessEngine.h"
class GameSFML : public Game {
public:
    GameSFML();
    void run();
    void playerTurn();
    bool isGameCompleted();

private:
    sf::RenderWindow window;
    Board board;
    BoardRenderer renderer;
    InputHandler inputHandler;
    float squareSize = 100.0f;
    std::vector<Position> highlightedMoves;
    std::optional<Position> selectedSquare;
    ConsoleUi console;
    std::string endMessage;
    bool gameEnded = false;
    std::optional<Position> kingInCheckPos;

    ChessEngine engine;

};