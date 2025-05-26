#pragma once
#include<iostream>
#include "../Board/Board.h"
#include "../Game/Game.h"
class ConsoleUi
{
public:
	
	static void displayBoard(const Board& board, Color player);
	PieceType askPromotionChoice();
};