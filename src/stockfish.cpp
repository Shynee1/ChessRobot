#include "stockfish.hpp"

namespace bp = boost::process;

Stockfish::Stockfish(chess::Board* board, StockfishSettings settings) {
	this->settings = settings;
	this->board = board;

	this->currentSearchDepth = "Book";
	this->currentScore = "Book";

	stockfish = bp::child(EXEC_PATH, bp::std_in < writeStream, bp::std_out > readStream);
	writeStream << "uci" << std::endl;
	writeStream << "setoption name Skill Level value " << settings.skillLevel << std::endl;
	writeStream << "setoption name Threads value " << settings.threads << std::endl;

	new_game();
	wait_for_ready();
}

Stockfish::~Stockfish() {
	stockfish.terminate();
}

void Stockfish::update() {
	if (!isGeneratingMoves) return;

	std::string line;
	std::getline(readStream, line);

	auto split = split_string(line, ' ');

	if (line.find("depth") != -1){
		this->currentSearchDepth = split[2];
		if (split[8] == "mate")
			this->currentScore = "Mate in " + std::to_string(stoi(split[9]) - 1);
		else
			this->currentScore = split[9];
		this->currentMove = split[21];
	}
	
	if (line.find("bestmove") != -1) {
		std::string uciMove = split[1];
	
		// Fix checkmate issue
		if (uciMove.length() > 4 && uciMove[4] == '\r'){
			uciMove = uciMove.substr(0, 4);
			this->currentScore = std::string("Checkmate");
		}

		bestMove = chess::uci::uciToMove(*board, uciMove);
		isGeneratingMoves = false;
	}
}

void Stockfish::wait_for_ready() {
	writeStream << "isready" << std::endl;
	std::string line;
	while (std::getline(readStream, line)) {
		if (line.find("readyok") != -1)
			break;
	}
}

void Stockfish::new_game() {
	writeStream << "ucinewgame" << std::endl;
	writeStream << "position fen " << STARTING_FEN << std::endl;
}

void Stockfish::generate_best_move() {
	writeStream << "position fen " << board->getFen() << std::endl;
	wait_for_ready();
	writeStream << "go depth " << settings.depth << " movetime " << settings.movetime << std::endl;

	isGeneratingMoves = true;
}

bool Stockfish::is_generating_moves() {
	return isGeneratingMoves;
}

chess::Move Stockfish::get_best_move() {
	return bestMove;
}

std::string Stockfish::get_search_depth() {
	return currentSearchDepth;
}

std::string Stockfish::get_score() {
	return currentScore;
}

std::string Stockfish::get_current_move() {
	return currentMove;
}