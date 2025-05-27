#include "ConsoleUI.h"
#include <map>
#include <limits>
#undef max
namespace {
    const std::map<std::pair<PieceType, Color>, char> pieceSymbols = {
        {{PieceType::Pawn, Color::White},   'P'},
        {{PieceType::Rook, Color::White},   'R'},
        {{PieceType::Knight, Color::White}, 'N'},
        {{PieceType::Bishop, Color::White}, 'B'},
        {{PieceType::Queen, Color::White},  'Q'},
        {{PieceType::King, Color::White},   'K'},
        {{PieceType::Pawn, Color::Black},   'p'},
        {{PieceType::Rook, Color::Black},   'r'},
        {{PieceType::Knight, Color::Black}, 'n'},
        {{PieceType::Bishop, Color::Black}, 'b'},
        {{PieceType::Queen, Color::Black},  'q'},
        {{PieceType::King, Color::Black},   'k'}
    };
}

void ConsoleUi::displayBoard(const Board& board, Color player) {
   
    std::cout << "Current player " << player << "\n";
    std::cout << "  a b c d e f g h\n";
    for (int y = 7; y >= 0; --y) {
        std::cout << (y + 1) << " ";
        for (int x = 0; x < 8; ++x) {
            Position pos(x, y);
            const Piece& piece = board.getPiece(pos);

            if (piece.getType() == PieceType::None) {
                std::cout << ((x + y) % 2 ? '.' : ' ') << " ";
            }
            else {
                auto symbol = pieceSymbols.at({ piece.getType(), piece.getColor() });
                std::cout << symbol << " ";
            }
        }
        std::cout << (y + 1) << "\n";
    }
    std::cout << "  a b c d e f g h\n";
}


PieceType ConsoleUi::askPromotionChoice() {
    std::cout << "Promocja pionka! Wybierz figurę:\n";
    std::cout << "1. Hetman (Q)\n";
    std::cout << "2. Wieza (R)\n";
    std::cout << "3. Goniec (B)\n";
    std::cout << "4. Skoczek (N)\n";
    int choice = 0;
    while (choice < 1 || choice > 4) {
        std::cout << "Twoj wybor (1-4): ";
        std::cin >> choice;
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            choice = 0;
        }
    }
    switch (choice) {
    case 1: return PieceType::Queen;
    case 2: return PieceType::Rook;
    case 3: return PieceType::Bishop;
    case 4: return PieceType::Knight;
    default: return PieceType::Queen;
    }
}

Color ConsoleUi::askPlayerColor()
{
    std::cout << "Choose color you want to play with: \n";
    std::cout << "1. White\n2. Black";
    int choice = 0;
    while (choice < 1 || choice > 2)
    {
        std::cin >> choice;
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            choice = 0;
        }
    }
    switch (choice)
    {
    case 1: return Color::White;
    case 2: return Color::Black;
    default: return Color::White;
    }
}