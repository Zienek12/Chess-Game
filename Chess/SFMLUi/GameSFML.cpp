#include "GameSFML.h"
#include <optional>
#include "InputHandler.h"
#include "../Pieces/Move.h"

// Constructor: Initializes the SFML window and sets the square size
GameSFML::GameSFML()
    : window(sf::VideoMode({ 800, 800 }), "Chess"),
    squareSize(100.0f)
{
    window.setFramerateLimit(30);
}

// Checks if the game has ended (checkmate, stalemate, or insufficient material)
bool GameSFML::isGameCompleted()
{
    // Checkmate condition
    if (board.isKingInCheck(currentPlayer) &&
        board.getAllLegalMoves(currentPlayer).empty())
    {
        endMessage = "Check mate! " + std::string(currentPlayer == Color::White ? "Black" : "White") + " wins";
        gameEnded = true;
        return true;
    }

    // Stalemate condition
    if (!board.isKingInCheck(currentPlayer) &&
        board.getAllLegalMoves(currentPlayer).empty())
    {
        endMessage = "Stalemate! Draw.";
        gameEnded = true;
        return true;
    }

    // Insufficient material condition
    if (board.isInsufficientMaterial())
    {
        endMessage = "Draw! Insufficient material.";
        gameEnded = true;
        return true;
    }

    return false;
}

// Draws the color selection screen
void GameSFML::drawColorSelection(sf::RenderWindow& window, sf::Font& font)
{
    sf::Text title(font);
    title.setString("Choose player color");
    title.setFillColor(sf::Color::White);
    title.setPosition(sf::Vector2f(180, 150));

    sf::RectangleShape whiteButton(sf::Vector2f(200, 80));
    whiteButton.setPosition(sf::Vector2f(150, 350));
    whiteButton.setFillColor(sf::Color(220, 220, 220));

    sf::Text whiteText(font);
    whiteText.setString("White");
    whiteText.setFillColor(sf::Color::Black);
    whiteText.setPosition(sf::Vector2f(whiteButton.getPosition().x + 50, whiteButton.getPosition().y + 15));

    sf::RectangleShape blackButton(sf::Vector2f(200, 80));
    blackButton.setPosition(sf::Vector2f(450, 350));
    blackButton.setFillColor(sf::Color(50, 50, 50));

    sf::Text blackText(font);
    blackText.setString("Black");
    blackText.setFillColor(sf::Color::White);
    blackText.setPosition(sf::Vector2f(blackButton.getPosition().x + 40, blackButton.getPosition().y + 15));

    this->window.draw(title);
    this->window.draw(whiteButton);
    this->window.draw(whiteText);
    this->window.draw(blackButton);
    this->window.draw(blackText);
}

// Shows the promotion dialog and returns the chosen piece type
PieceType GameSFML::showPromotionDialog()
{
    sf::RectangleShape buttons[4];
    std::string labels[4] = { "Queen", "Rook", "Bishop", "Knight" };
    PieceType types[4] = { PieceType::Queen, PieceType::Rook, PieceType::Bishop, PieceType::Knight };

    float startX = 200, startY = 300, width = 100, height = 60, spacing = 30;

    sf::Font font;
    // Load font for displaying promotion options
    if (!font.openFromFile("../../../../Chess/Assets/arial.ttf"))
    {
        std::cerr << "Failed to load font: arial.ttf" << std::endl;
        return PieceType::Queen;
    }

    for (int i = 0; i < 4; ++i)
    {
        buttons[i].setSize(sf::Vector2f(width, height));
        buttons[i].setPosition(sf::Vector2f(startX + i * (width + spacing), startY));
        buttons[i].setFillColor(sf::Color(200, 200, 200));
    }

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
                return PieceType::Queen;
            }
            if (event->is<sf::Event::MouseButtonPressed>())
            {
                auto mousePos = sf::Mouse::getPosition(window);
                for (int i = 0; i < 4; ++i)
                {
                    if (buttons[i].getGlobalBounds().contains(sf::Vector2f(mousePos)))
                    {
                        return types[i];
                    }
                }
            }
        }

        window.clear(sf::Color::Black);
        for (int i = 0; i < 4; ++i)
        {
            window.draw(buttons[i]);
            sf::Text text(font);
            text.setString(labels[i]);
            text.setFillColor(sf::Color::Black);
            text.setPosition(buttons[i].getPosition() + sf::Vector2f(10, 10));
            window.draw(text);
        }
        window.display();
    }
    return PieceType::Queen;
}

// Handles the player's turn and input
void GameSFML::playerTurn()
{
    while (const std::optional event = window.pollEvent())
    {
        // Handle window close event
        if (event->is<sf::Event::Closed>())
        {
            window.close();
            continue;
        }

        // Ignore input if game ended or event is not mouse button press
        if (gameEnded || !event->is<sf::Event::MouseButtonPressed>())
        {
            continue;
        }

        // Handle mouse click and get board position
        auto pos = inputHandler.handleEvent(*event, window);
        if (!pos) continue;

        const auto piece = board.getPiece(*pos);

        // Select or change selected square
        if (!selectedSquare)
        {
            if (piece.getColor() == currentPlayer)
            {
                selectedSquare = pos;
                highlightedMoves.clear();
                const auto allLegalMoves = board.getAllLegalMoves(currentPlayer);
                auto it = allLegalMoves.find(*selectedSquare);
                if (it != allLegalMoves.end())
                {
                    for (const auto& move : it->second)
                    {
                        highlightedMoves.push_back(move.to);
                    }
                }
            }
            continue;
        }

        // Deselect if the same square is clicked
        if (*selectedSquare == *pos)
        {
            selectedSquare.reset();
            highlightedMoves.clear();
            continue;
        }

        // Change selection if another piece of the current player is clicked
        if (piece.getColor() == currentPlayer)
        {
            selectedSquare = pos;
            highlightedMoves.clear();
            const auto allLegalMoves = board.getAllLegalMoves(currentPlayer);
            auto it = allLegalMoves.find(*selectedSquare);
            if (it != allLegalMoves.end())
            {
                for (const auto& move : it->second)
                {
                    highlightedMoves.push_back(move.to);
                }
            }
            continue;
        }

        // Attempt to make a move
        Move move(*selectedSquare, *pos);
        const auto selectedPiece = board.getPiece(*selectedSquare);

        // Handle pawn promotion
        if (selectedPiece.getType() == PieceType::Pawn)
        {
            if ((selectedPiece.getColor() == Color::White && pos->y == 7) ||
                (selectedPiece.getColor() == Color::Black && pos->y == 0))
            {
                move.promotionType = showPromotionDialog();
            }
        }

        if (board.movePiece(move))
        {
            system("cls");
            selectedSquare.reset();
            highlightedMoves.clear();
            currentPlayer = (currentPlayer == Color::Black) ? Color::White : Color::Black;
            console.displayBoard(board, currentPlayer);
        }
        else
        {
            selectedSquare.reset();
            highlightedMoves.clear();
        }

        // Update king in check highlight
        if (board.isKingInCheck(currentPlayer))
        {
            kingInCheckPos = board.findKingPos(currentPlayer);
        }
        else
        {
            kingInCheckPos.reset();
        }
    }
}

// Main game loop
void GameSFML::run()
{
    sf::Font font;
    // Load font for displaying end game messages
    if (!font.openFromFile("../../../../Chess/Assets/arial.ttf"))
    {
        std::cerr << "Failed to load font: arial.ttf" << std::endl;
        return;
    }

    console.displayBoard(board, currentPlayer);

    while (window.isOpen())
    {
        // Handle color selection at the start of the game
        if (!colorSelectionDone)
        {
            window.clear();
            drawColorSelection(window, font);
            window.display();

            // Handle color selection events
            while (const std::optional event = window.pollEvent())
            {
                if (event->is<sf::Event::Closed>())
                {
                    window.close();
                    break;
                }

                if (event->is<sf::Event::MouseButtonPressed>())
                {
                    auto mousePos = sf::Mouse::getPosition(window);
                    auto mousePosF = sf::Vector2f(mousePos);

                    sf::FloatRect whiteButtonRect({ 150.f, 350.f }, { 200.f, 80.f });
                    sf::FloatRect blackButtonRect({ 450.f, 350.f }, { 200.f, 80.f });

                    if (whiteButtonRect.contains(mousePosF))
                    {
                        // White is chosen
                        playerColor = Color::White;
                        aiColor = Color::Black;
                        currentPlayer = Color::White;
                        colorSelectionDone = true;
                        break;
                    }
                    else if (blackButtonRect.contains(mousePosF))
                    {
                        // Black is chosen
                        playerColor = Color::Black;
                        aiColor = Color::White;
                        currentPlayer = Color::White;
                        colorSelectionDone = true;
                        break;
                    }
                }
            }
            continue;
        }
        if (colorSelectionDone && playerColor == Color::Black && !aiMoveAfterColorSelectionDone)
        {
            window.clear();
            renderer.draw(window, board, selectedSquare, highlightedMoves, kingInCheckPos);
            window.display();

            board.movePiece(engine.findBestMoveAlphaBeta(board, 3, aiColor));
            currentPlayer = playerColor; 
            aiMoveAfterColorSelectionDone = true;
            continue;

        }

        // Check if the game has ended
        if (!gameEnded)
        {
            isGameCompleted();
        }

        // Handle player or AI turn
        if (currentPlayer == playerColor)
        {
            playerTurn();
        }
        else
        {
            board.movePiece(engine.findBestMoveAlphaBeta(board, 3, aiColor));
            currentPlayer = (currentPlayer == Color::Black) ? Color::White : Color::Black;
            std::cout << "\n\nVisited nodes: " << engine.getNodesVisited() << std::endl;
        }

        // Draw the chessboard and pieces
        window.clear();
        renderer.draw(window, board, selectedSquare, highlightedMoves, kingInCheckPos);

        // Draw end game message if the game has ended
        if (gameEnded)
        {
            sf::Text text(font);
            text.setString(endMessage);
            text.setCharacterSize(48);
            text.setFillColor(sf::Color::Red);
            text.setStyle(sf::Text::Bold);

            sf::FloatRect textRect = text.getLocalBounds();
            text.setOrigin(textRect.getCenter());
            text.setPosition(sf::Vector2f(window.getSize().x / 2.0f, window.getSize().y / 2.0f));

            window.draw(text);
        }

        window.display();
    }
}
