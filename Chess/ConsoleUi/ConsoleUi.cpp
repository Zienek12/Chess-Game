#include "ConsoleUI.h"
#include "../Pieces/PieceType.h"
#include "../Pieces/Color.h"
#include <map>

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

void ConsoleUi::displayBoard(const Board& board) {
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