#pragma once
#include "../Board/Board.h"
#include "../Pieces/Position.h"
#include <SFML/Graphics.hpp>
#include <map>
#include <string>
#include "../Pieces/Piece.h"
class BoardRenderer
{
public:
	BoardRenderer(float squareSize = 100.f);
	void draw(sf::RenderWindow& window, const Board& board, 
		std::optional<Position> selectedSquare, 
		const std::vector<Position>& highlightedMoves,
		const std::optional<Position> kingInCheckPos);
private:
	float squareSize;

	std::map<std::string, sf::Texture> pieceTextures;

	void loadTextures();
	std::string getPieceTextureKey(const Piece& piece) const;

	sf::Color getSquareColor(const Position& pos)const;
};