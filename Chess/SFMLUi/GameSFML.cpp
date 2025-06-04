#include "GameSFML.h"
#include <optional>
#include "InputHandler.h"
#include "../Pieces/Move.h"

// Constructor: Initializes the SFML window and sets the square size
GameSFML::GameSFML()
    :
    window(sf::VideoMode({ 800, 800 }), "Chess"),
    squareSize(100.0f)
{
    window.setFramerateLimit(30);
}

// Checks if the game has ended (checkmate, stalemate, or insufficient material)
bool GameSFML::isGameCompleted() {
    // Checkmate condition
    if (board.isKingInCheck(currentPlayer) &&
        board.getAllLegalMoves(currentPlayer).empty()) {
        endMessage = "Check mate! " + std::string(currentPlayer == Color::White ? "Black" : "White") + " wins";
        gameEnded = true;
        return true;
    }
    // Stalemate condition
    if (!board.isKingInCheck(currentPlayer) &&
        board.getAllLegalMoves(currentPlayer).empty()) {
        endMessage = "Stalemate! Draw.";
        gameEnded = true;
        return true;
    }
    // Insufficient material condition
    if (board.isInsufficientMaterial()) {
        endMessage = "Draw! Insufficient material.";
        gameEnded = true;
        return true;
    }
    return false;
}

void GameSFML::drawColorSelection(sf::RenderWindow& window, sf::Font& font) {
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

    this->window.draw(title); // Use this-> to refer to the class member
    this->window.draw(whiteButton);
    this->window.draw(whiteText);
    this->window.draw(blackButton);
    this->window.draw(blackText);
}



void GameSFML::playerTurn()
{
    // Handle window and input events
    while (const std::optional event = window.pollEvent()) {
        // Handle window close event
        if (event->is<sf::Event::Closed>()) {
            window.close();
            continue;
        }

        // Ignore input if game ended or event is not mouse button press
        if (gameEnded || !event->is<sf::Event::MouseButtonPressed>()) {
            continue;
        }

        // Handle mouse click and get board position
        auto pos = inputHandler.handleEvent(*event, window);
        if (!pos) continue;

        const auto piece = board.getPiece(*pos);

        // Select or change selected square
        if (!selectedSquare) {
            if (piece.getColor() == currentPlayer) {
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

        // Deselect if the same square is clicked
        if (*selectedSquare == *pos) {
            selectedSquare.reset();
            highlightedMoves.clear();
            continue;
        }

        // Change selection if another piece of the current player is clicked
        if (piece.getColor() == currentPlayer) {
            selectedSquare = pos;
            highlightedMoves.clear();
            const auto allLegalMoves = board.getAllLegalMoves(currentPlayer);
            auto it = allLegalMoves.find(*selectedSquare);
            if (it != allLegalMoves.end()) {
                for (const auto& move : it->second) {
                    highlightedMoves.push_back(move.to);
                }
            }
            continue;
        }

        // Attempt to make a move
        Move move(*selectedSquare, *pos);
        if (board.movePiece(move)) {
            system("cls");
            selectedSquare.reset();
            highlightedMoves.clear();
            currentPlayer = (currentPlayer == Color::Black) ? Color::White : Color::Black;
            console.displayBoard(board, currentPlayer);
        }
        else {
            selectedSquare.reset();
            highlightedMoves.clear();
        }

        if (board.isKingInCheck(currentPlayer)) {
            kingInCheckPos = board.findKingPos(currentPlayer);
        }
        else
        {
            kingInCheckPos.reset();
        }

    }
}


// Main game loop
void GameSFML::run() {
    sf::Font font;
    // Load font for displaying end game messages
    if (!font.openFromFile("../../../../Chess/Assets/arial.ttf")) {
        std::cerr << "Failed to load font: arial.ttf" << std::endl;
        return;
    }

    console.displayBoard(board, currentPlayer);
   

    while (window.isOpen()) {
        // Check if the game has ended
        if (!colorSelectionDone) {
            // Rysuj i obs?uguj wybór koloru
            window.clear();
            drawColorSelection(window, font);
            window.display();

            // Tylko obs?uga zdarze? wyboru koloru
            while (const std::optional event = window.pollEvent()) {
                if (event->is<sf::Event::Closed>()) {
                    window.close();
                    break;
                }

                if (event->is<sf::Event::MouseButtonPressed>()) {
                    auto mousePos = sf::Mouse::getPosition(window);
                    auto mousePosF = sf::Vector2f(mousePos);

                    sf::FloatRect whiteButtonRect({ 150.f, 350.f }, { 200.f, 80.f });
                    sf::FloatRect blackButtonRect({ 450.f, 350.f }, { 200.f, 80.f });

                    if (whiteButtonRect.contains(mousePosF)) {
                        // white is chosen
                        playerColor = Color::White;
                        aiColor = Color::Black;
                        currentPlayer = Color::White;
                        colorSelectionDone = true;
                        break;
                    }
                    else if (blackButtonRect.contains(mousePosF)) {
                        // black is chosen
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








        if (!gameEnded) {
            isGameCompleted();
        }

        if (currentPlayer == playerColor)
        {
            playerTurn();
           
        }
        else
        {
            board.movePiece(engine.findBestMoveAlphaBeta(board, 3, aiColor));
            currentPlayer = (currentPlayer == Color::Black) ? Color::White : Color::Black;
            std::cout << "\n\nOdwiedzone wezly: " << engine.getNodesVisited() << std::endl;

        }
        // Draw the chessboard and pieces
       
        window.clear();
        renderer.draw(window, board, selectedSquare, highlightedMoves, kingInCheckPos);
        // Draw end game message if the game has ended
        if (gameEnded) {
            sf::Text text(font);
            text.setString(endMessage);
            text.setCharacterSize(48);
            text.setFillColor(sf::Color::Red);
            text.setStyle(sf::Text::Bold);

            sf::FloatRect textRect = text.getLocalBounds();
            text.setOrigin(
                textRect.getCenter()
            );

            text.setPosition(sf::Vector2f(window.getSize().x / 2.0f, window.getSize().y / 2.0f));

            window.draw(text);
        }

        window.display();
    }
}
