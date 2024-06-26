#include "mouseinput.hpp"

using namespace chess;

MouseInput::MouseInput(chess::Board* board, BoardUI* ui, OpeningBook* book) {
	this->board = board;
	this->window = Window::Instance();
	this->ui = ui;
	this->book = book;
	
	movegen::legalmoves(moves, *board);
}

void MouseInput::update(float dt) {
	if (!window->is_mouse_pressed()) {
		wasMousePressed = false;
		return;
	}

	if (wasMousePressed || board->sideToMove() != Color::WHITE)
		return;

	SDL_Point mousePos = window->get_mouse_pos();
	int arrayIdx = ui->worldToArray(mousePos);

	if (!(arrayIdx >= 0 && arrayIdx < 64)) {
		wasMousePressed = true;
		return;
	}

	bool hasClicked = selectedPieceIdx != -1;

	if (!hasClicked) handle_first_click(arrayIdx);
	else handle_move_click(arrayIdx);

	this->wasMousePressed = true;
}

void MouseInput::handle_first_click(int clickedSquare) {
	Piece selectedPiece = board->at(clickedSquare);
	if (selectedPiece.type() == PieceType::NONE || selectedPiece.color() != board->sideToMove())
		return;

	ui->clear_ui();
	selectedPieceIdx = clickedSquare;
	ui->color_square(clickedSquare, ui->clicked_color());
	color_legal_moves(moves, clickedSquare);
}

void MouseInput::handle_move_click(int clickedSquare) {
	Move move = get_legal_move(moves, selectedPieceIdx, clickedSquare);

	ui->clear_ui();
	ui->color_square(clickedSquare, ui->clicked_color());
	selectedPieceIdx = -1;

	if (move == NULL) {
		ui->clear_ui(); return;
	}

	board->makeMove(move);
	book->updateMoves(board->hash());
	movegen::legalmoves(moves, *board);
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
	movegen::legalmoves(moves, *board);
	for (Move& move : moves) {
		if (move.from().index() == squareIdx) {
			ui->color_square(move.to().index(), ui->move_color());
		}
	}
}