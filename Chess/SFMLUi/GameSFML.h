#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "BoardRenderer.h"
#include "InputHandler.h"
#include "../Board/Board.h"
#include "../Game/Game.h"

class GameSFML : public Game {
public:
    GameSFML();
    void run();

private:
    sf::RenderWindow window;
    Board board;
    BoardRenderer renderer;
    InputHandler inputHandler;
    float squareSize = 100.0f;
    std::vector<Position> highlightedMoves;
    std::optional<Position> selectedSquare;
};