# CMakeList.txt: projekt CMake dla elementu Chess, dołącz źródło i zdefiniuj
# tutaj logikę specyficzną dla projektu.
#

# Dodaj źródło do pliku wykonywalnego tego projektu.
add_executable (Chess "main.cpp" "Pieces/Piece.h" "Pieces/PieceType.h" "Pieces/Color.h" "Pieces/Position.h" "Board/Board.h" "Board/Board.cpp" "ConsoleUi/ConsoleUi.h" "ConsoleUi/ConsoleUi.cpp" "HandlePlayerInput/HandlePlayerInput.h" "HandlePlayerInput/HandlePlayerInput.cpp" "Game/Game.h" "Game/Game.cpp" "Pieces/Pawn/Pawn.h" "Pieces/Pawn/Pawn.cpp")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Chess PROPERTY CXX_STANDARD 20)
endif()

# TODO: Dodaj testy i zainstaluj elementy docelowe w razie potrzeby.
