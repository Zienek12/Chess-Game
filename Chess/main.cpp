#include "Board/Board.h"
#include "ConsoleUi/ConsoleUi.h"


int main()
{

	Board standardBoard("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
	ConsoleUi::displayBoard(standardBoard);


}