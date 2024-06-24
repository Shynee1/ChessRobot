#pragma once
#include "utility.hpp"
#include "BoardUI.hpp"
#include "openingbook.hpp"

class MouseInput {
private:
	chess::Board* board;
	chess::Movelist moves;

	OpeningBook* book;
	BoardUI* ui;
	Window* window;

	int selectedPieceIdx = -1;
	bool wasMousePressed = false;
public:
	MouseInput(chess::Board* board, Window* window, BoardUI* ui, OpeningBook* book);
	void update(float deltaTime);
	void handle_first_click(int clickedSquare);
	void handle_move_click(int clickedSquare);
	chess::Move get_legal_move(chess::Movelist& moves, int from, int to);
	void color_legal_moves(chess::Movelist& moves, int squareIdx);
};