#include "stockfish.hpp"

namespace bp = boost::process;

Stockfish::Stockfish(chess::Board* board, StockfishSettings settings) {
	this->settings = settings;
	this->board = board;

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

static void printchar(unsigned char theChar) {

	switch (theChar) {

	case '\n':
		printf("\\n\n");
		break;
	case '\r':
		printf("\\r");
		break;
	case '\t':
		printf("\\t");
		break;
	default:
		if ((theChar < 0x20) || (theChar > 0x7f)) {
			printf("\\%03o", (unsigned char)theChar);
		}
		else {
			printf("%c", theChar);
		}
		break;
	}
}

void Stockfish::update() {
	if (!isGeneratingMoves) return;

	std::string line;
	std::getline(readStream, line);

	std::cout << line << "\n";

	if (line.find("bestmove") == -1) return;

	std::vector<std::string> split = split_string(line, ' ');
	std::string uciMove = split[1];
	
	// Fix checkmate issue
	if (uciMove.length() > 4 && uciMove[4] == '\r')
		uciMove = uciMove.substr(0, 4);

	bestMove = chess::uci::uciToMove(*board, uciMove);
	isGeneratingMoves = false;
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