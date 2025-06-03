#pragma once
#include "../Pieces/Position.h"
#include <optional>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

class InputHandler
{
public:
	InputHandler() : squareSize(100.0f) {}

	InputHandler(float squareSize);
	std::optional<Position> handleEvent(const sf::Event& event, const sf::RenderWindow& window);
private:
	float squareSize = 100.0f;
};