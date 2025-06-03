#include "BoardRenderer.h"
#include <stdexcept>
#include <SFML/Graphics.hpp> 
#include <iostream>

BoardRenderer::BoardRenderer(float squareSize) : squareSize(squareSize)
{
    loadTextures();
}



sf::Color BoardRenderer::getSquareColor(const Position& pos) const {
    return (pos.x + pos.y) % 2 == 0 ? sf::Color(240, 217, 181) : sf::Color(181, 136, 99);
}


void BoardRenderer::loadTextures()
{
    const char* colors[] = { "W", "B" };
    const char* types[] = { "P", "N", "B", "R", "Q", "K" };

    for (const char* color : colors)
    {
        for (const char* type : types)
        {
            std::string key = std::string(color) + type;
            std::string path = "../../../../Chess/Assets/" + key + ".png";
            sf::Texture texture;
            if (texture.loadFromFile(path))
            {
                pieceTextures[key] = std::move(texture);
            }
            else
            {
                std::cout << "Nie mozna zaladowac tekstury: " << path << std::endl;
            }

        }
    }
}


std::string BoardRenderer::getPieceTextureKey(const Piece& piece) const {
    std::string color = (piece.getColor() == Color::White) ? "W" : "B";
    std::string type;
    switch (piece.getType()) {
    case PieceType::Pawn:   type = "P"; break;
    case PieceType::Knight: type = "N"; break;
    case PieceType::Bishop: type = "B"; break;
    case PieceType::Rook:   type = "R"; break;
    case PieceType::Queen:  type = "Q"; break;
    case PieceType::King:   type = "K"; break;
    default: return "";
    }
    return color + type;
}


void BoardRenderer::draw(sf::RenderWindow& window, const Board& board, 
    std::optional<Position> selectedSquare,const std::vector<Position>& highlightedMoves) {
    // Draw the chessboard and pieces
    for (int x = 0; x < 8; ++x) {
        for (int y = 0; y < 8; ++y) {
            Position pos(x, y);

            // Calculate the pixel position: (0,0) is bottom-left, (0,7) is top-left
            float pixelX = static_cast<float>(x) * squareSize;
            float pixelY = static_cast<float>(7 - y) * squareSize;

            // Draw the square
            sf::RectangleShape square({ squareSize, squareSize });
            square.setPosition(sf::Vector2f(pixelX, pixelY));
            square.setFillColor(getSquareColor(pos));
            window.draw(square);

            // Draw the piece if present
            const Piece& piece = board.getPiece(pos);
            std::string key = getPieceTextureKey(piece);
            if (!key.empty() && pieceTextures.count(key)) {
                sf::Sprite sprite(pieceTextures.at(key));
                sprite.setPosition(sf::Vector2f(pixelX, pixelY));
                sprite.setScale(sf::Vector2f(
                    squareSize / sprite.getTexture().getSize().x,
                    squareSize / sprite.getTexture().getSize().y
                ));
                window.draw(sprite);
            }

            // Draw highlight if this square is selected
            if (selectedSquare && selectedSquare->x == x && selectedSquare->y == y) {
                sf::RectangleShape highlight(sf::Vector2f(squareSize, squareSize));
                highlight.setPosition(sf::Vector2f(pixelX, pixelY));
                highlight.setFillColor(sf::Color(255, 255, 0, 100)); // semi-transparent yellow
                window.draw(highlight);
            }

            for (const auto& movePos : highlightedMoves) {
                if (movePos.x == x && movePos.y == y) {
                    sf::CircleShape moveHighlight(squareSize / 4);
                    moveHighlight.setFillColor(sf::Color(0, 255, 0, 150)); // semi-transparent green
                    moveHighlight.setOrigin(sf::Vector2f(moveHighlight.getRadius(), moveHighlight.getRadius()));
                    moveHighlight.setPosition(sf::Vector2f(pixelX + squareSize / 2, pixelY + squareSize / 2));
                    window.draw(moveHighlight);
                }
            }

        }
    }
}
