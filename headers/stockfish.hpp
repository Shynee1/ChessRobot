#pragma once
#include "utility.hpp"
#include <boost/process.hpp>

constexpr char EXEC_PATH[] = "C:\\vclib\\stockfish\\stockfish-windows-x86-64.exe";

struct StockfishSettings {
	int skillLevel;
	int depth;
	int movetime;
	int threads;

	StockfishSettings(int skillLevel, int depth, int movetime, int threads)
		: skillLevel(skillLevel), depth(depth), movetime(movetime), threads(threads){}

	StockfishSettings() = default;
};

class Stockfish {
private:
	chess::Board* board;
	chess::Move bestMove;
	boost::process::ipstream readStream;
	boost::process::opstream writeStream;
	boost::process::child stockfish;
	StockfishSettings settings;
	bool isGeneratingMoves = false;
	std::string currentSearchDepth;
	std::string currentScore;
	std::string currentMove;
public:
	Stockfish(chess::Board* board, StockfishSettings settings);
	~Stockfish();
	void update();
	void wait_for_ready();
	void new_game();
	void generate_best_move();
	bool is_generating_moves();
	chess::Move get_best_move();
	std::string get_search_depth();
	std::string get_score();
	std::string get_current_move();
};