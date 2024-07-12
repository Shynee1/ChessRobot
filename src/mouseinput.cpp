#include "mouseinput.hpp"

using namespace chess;

void MouseInput::start() {
	this->board = GameManager::Instance()->get_board();
	movegen::legalmoves(moves, *board);
}

void MouseInput::update() {
	if (!Window::Instance()->is_mouse_pressed()) {
		wasMousePressed = false;
		return;
	}

	if (wasMousePressed || board->sideToMove() != Color::WHITE)
		return;

	SDL_Point mousePos = Window::Instance()->get_mouse_pos();
	int arrayIdx = worldToArray(mousePos);

	if (arrayIdx <= 0 || arrayIdx > 64) {
		wasMousePressed = true;
		return;
	}

	bool hasClicked = selectedPieceIdx != -1;

	if (!hasClicked) handle_first_click(arrayIdx);
	else handle_move_click(arrayIdx);

	this->wasMousePressed = true;
}

void MouseInput::graphics() {
	
}

void MouseInput::handle_first_click(int clickedSquare) {
	auto ui = GameManager::Instance()->get_component<BoardUI>();
	Piece selectedPiece = board->at(clickedSquare);
	if (selectedPiece.type() == PieceType::NONE || selectedPiece.color() != board->sideToMove())
		return;

	ui->clear_ui();
	selectedPieceIdx = clickedSquare;
	ui->color_square(clickedSquare, ui->clicked_color());
	color_legal_moves(moves, clickedSquare);
}

void MouseInput::handle_move_click(int clickedSquare) {
	auto ui = GameManager::Instance()->get_component<BoardUI>();
	Move move = get_legal_move(moves, selectedPieceIdx, clickedSquare);

	ui->clear_ui();
	ui->color_square(clickedSquare, ui->clicked_color());
	selectedPieceIdx = -1;

	if (move == NULL) {
		ui->clear_ui(); return;
	}

	board->makeMove(move);
	movegen::legalmoves(moves, *board);
	GameManager::Instance()->get_opening_book()->updateMoves(board->hash());
}

Move MouseInput::get_legal_move(Movelist& moves, int from, int to) {
	movegen::legalmoves(moves, *board);
	for (Move& move : moves) {
		if (move.from().index() == from && move.to().index() == to)
			return move;
	}
	return NULL;
}


void MouseInput::color_legal_moves(Movelist& moves, int squareIdx) {
	auto ui = GameManager::Instance()->get_component<BoardUI>();
	movegen::legalmoves(moves, *board);
	for (Move& move : moves) {
		if (move.from().index() == squareIdx) {
			ui->color_square(move.to().index(), ui->move_color());
		}
	}
}