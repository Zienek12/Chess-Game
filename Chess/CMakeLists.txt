set(SFML_DIR "C:\\Software\\SFML-3.0.0\\lib\\cmake\\SFML")
find_package(SFML 3 REQUIRED COMPONENTS Graphics System Window)

add_library(ChessLib
    "Pieces/Piece.h" "Pieces/PieceType.h" "Pieces/Color.h" "Pieces/Position.h"
    "Board/Board.h" "Board/Board.cpp"
    "ConsoleUi/ConsoleUi.h" "ConsoleUi/ConsoleUi.cpp"
    "HandlePlayerInput/HandlePlayerInput.h" "HandlePlayerInput/HandlePlayerInput.cpp"
    "Game/Game.h" "Game/Game.cpp"
    "Pieces/Pawn/Pawn.h" "Pieces/Pawn/Pawn.cpp"
    "Pieces/Knight/Knight.h" "Pieces/Knight/Knight.cpp"
    "Pieces/Rook/Rook.h" "Pieces/Rook/Rook.cpp"
    "Pieces/Bishop/Bishop.h" "Pieces/Bishop/Bishop.cpp"
    "Pieces/Queen/Queen.h" "Pieces/Queen/Queen.cpp"
    "Pieces/King/King.h" "Pieces/King/King.cpp"
    "SFMLUi/BoardRenderer.h" "SFMLUi/BoardRenderer.cpp"
    "Pieces/Move.h"
    "ChessEngine/ChessEngine.h" "ChessEngine/ChessEngine.cpp"
    "Board/MoveState.h"
    "SFMLUi/InputHandler.h" "SFMLUi/InputHander.cpp"
    "SFMLUi/GameSFML.h" "SFMLUi/GameSFML.cpp"
)
target_link_libraries(ChessLib PUBLIC SFML::Graphics SFML::Window SFML::System)

add_executable (Chess "main.cpp")
target_link_libraries(Chess PRIVATE ChessLib)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Chess PROPERTY CXX_STANDARD 20)
endif()



add_executable(Tests "Tests/StandardBoardTests.cpp" "Tests/MockTests.cpp" "Tests/ParametrialTests.cpp")
target_link_libraries(Tests PRIVATE ChessLib GTest::gtest_main GTest::gmock_main)


include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/5376968f6948923e2411081fd9372e71a59d8e77.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

include(GoogleTest)
gtest_discover_tests(Tests)