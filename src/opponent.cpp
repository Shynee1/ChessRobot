#include "opponent.hpp"

void Opponent::start() {
	this->gui = GUI::Instance();
	this->board = GameManager::Instance()->get_board();
	this->book = GameManager::Instance()->get_opening_book();
	this->motorController = GameManager::Instance()->get_component<MotorController>();
	this->boardUI = GameManager::Instance()->get_component<BoardUI>();
	this->stockfish = std::make_unique<Stockfish>(board);
	set_stockfish_level(level);
	this->stockfish->new_game();
}

void Opponent::update() {
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

void Opponent::graphics(){
	gui->get_label("bestMove").set_text("Best Move: " + get_best_move_text());
	gui->get_label("depth").set_text("Depth: " + stockfish->get_search_depth());
	gui->get_label("score").set_text("Evaluation: " + stockfish->get_score());
}

void Opponent::play_move(chess::Move& move) {
	motorController->send_move(move);
	boardUI->clear_ui();
	board->makeMove(move);
	boardUI->color_square(move.from().index(), boardUI->clicked_color());
	boardUI->color_square(move.to().index(), boardUI->clicked_color());
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

void Opponent::set_stockfish_level(int level) {
	this->level = level;
	
	StockfishSettings setting = levelMap[level];
	if (GameManager::Instance()->is_game_running())
		stockfish->set_settings(setting);

	auto gui = GUI::Instance();
	gui->get_label("skillLevel").set_text("Skill Level: " + std::to_string(setting.skillLevel));
	gui->get_label("targetDepth").set_text("Target Depth: " + std::to_string(setting.depth));
	gui->get_label("thinkTime").set_text("Think Time: " + std::to_string(setting.movetime));
}