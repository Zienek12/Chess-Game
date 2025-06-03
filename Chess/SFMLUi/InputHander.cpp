#include "InputHandler.h"

InputHandler::InputHandler(float squareSize) : squareSize(squareSize) {}

std::optional<Position> InputHandler::handleEvent(const sf::Event& event, const sf::RenderWindow& window)
{
    (void)window;
    if (squareSize <= 0.0f) return std::nullopt;

    if (event.is<sf::Event::MouseButtonPressed>()) {
        const auto* mouseEvent = event.getIf<sf::Event::MouseButtonPressed>();
        if (mouseEvent && mouseEvent->button == sf::Mouse::Button::Left) {
            int boardX = static_cast<int>(mouseEvent->position.x / squareSize);
            int boardY = static_cast<int>(mouseEvent->position.y / squareSize);
            Position pos(boardX, 7 - boardY);
            if (pos.isValid()) {
                return pos;
            }
        }
    }
    return std::nullopt;
}
