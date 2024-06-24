#include "opponent.hpp"

Opponent::Opponent(chess::Board* board, BoardUI* ui, OpeningBook* book, MotorController* mc, int level) {
	this->board = board;
	this->level = level;
	this->ui = ui;
	this->book = book;
	this->motorController = mc;
	this->stockfish = std::make_unique<Stockfish>(board, levelMap[level]);
}

void Opponent::update(float deltaTime) {
	if (board->sideToMove() != color) return;

	if (play_book_move()) return;
	
	if (stockfish->is_generating_moves()) {
		stockfish->update();
	}
	else if (!generatedMoves){
		stockfish->generate_best_move();
		generatedMoves = true;
	}
	else {
		chess::Move bestMove = stockfish->get_best_move();
		play_move(bestMove);
		generatedMoves = false;
	}
	
}

void Opponent::play_move(chess::Move move) {
	motorController->send_move(move);
	ui->clear_ui();
	board->makeMove(move);
	ui->color_square(move.from().index(), ui->clicked_color());
	ui->color_square(move.to().index(), ui->clicked_color());
	book->updateMoves(board->hash());
}

bool Opponent::play_book_move() {
	chess::Move bestMove = book->getRandomMove();
	if (bestMove == chess::Move::NO_MOVE)
		return false;
	
	play_move(bestMove);
	return true;
}