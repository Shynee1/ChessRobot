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

void Opponent::graphics() {
	TTF_Font* arial = AssetPool::Instance()->get_font("arial24");
	std::string bestMoveText = "Best Move: " + get_best_move_text();
	std::string depthText = "Depth: " + stockfish->get_search_depth();
	std::string scoreText = "Score: " + stockfish->get_score();

	Window::Instance()->render_text(BESTMOVE_TEXT_POS, bestMoveText, arial, BLACK);
	Window::Instance()->render_text(DEPTH_TEXT_POS, depthText, arial, BLACK);
	Window::Instance()->render_text(SCORE_TEXT_POS, scoreText, arial, BLACK);
}

void Opponent::play_move(chess::Move& move) {
	motorController->send_move(move);
	ui->clear_ui();
	board->makeMove(move);
	ui->color_square(move.from().index(), ui->clicked_color());
	ui->color_square(move.to().index(), ui->clicked_color());
	book->updateMoves(board->hash());
	lastMove = move;
}

bool Opponent::play_book_move() {
	chess::Move bookMove = book->getRandomMove();
	if (bookMove == chess::Move::NO_MOVE)
		return false;
	
	play_move(bookMove);
	return true;
}

std::string Opponent::get_best_move_text() {
	if (stockfish->is_generating_moves())
		return std::string("Thinking...");
	else if (lastMove != chess::Move::NO_MOVE)
		return chess::uci::moveToUci(lastMove);
	else
		return std::string("N/A");
}