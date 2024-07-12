#pragma once
#include "utility.hpp"
#include "gamemanager.hpp"
#include "window.hpp"
#include "boardui.hpp"

class MouseInput : public Component {
private:
	std::shared_ptr<chess::Board> board;
	chess::Movelist moves;

	int selectedPieceIdx = -1;
	bool wasMousePressed = false;
public:
	MouseInput() = default;
	void start();
	void update();
	void graphics();
private:
	void handle_first_click(int clickedSquare);
	void handle_move_click(int clickedSquare);
	chess::Move get_legal_move(chess::Movelist& moves, int from, int to);
	void color_legal_moves(chess::Movelist& moves, int squareIdx);
};