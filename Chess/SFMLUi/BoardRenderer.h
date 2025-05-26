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
	void draw(sf::RenderWindow& window, const Board& board);
private:
	float squareSize;
	sf::Font font;
	bool fontLoaded = false;
	std::map<std::string, sf::Texture> pieceTextures;

	void loadTextures();
	std::string getPieceTextureKey(const Piece& piece) const;

	sf::Color getSquareColor(const Position& pos)const;
};