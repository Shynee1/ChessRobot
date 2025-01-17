#pragma once
#include "utility.hpp"
#include <boost/process.hpp>

constexpr char EXEC_PATH[] = "C:\\vclib\\stockfish\\stockfish-windows-x86-64.exe";

struct StockfishSettings {
	int elo;
	int depth;
	int movetime;
	int threads;

	StockfishSettings(int elo, int depth, int movetime, int threads)
		: elo(elo), depth(depth), movetime(movetime), threads(threads){}

	StockfishSettings() = default;
};

class Stockfish {
private:
	boost::process::child stockfish;
	boost::process::ipstream readStream;
	boost::process::opstream writeStream;
	
	std::shared_ptr<chess::Board> board;
	chess::Move bestMove;
	StockfishSettings settings;
	bool isGeneratingMoves = false;
	std::string currentSearchDepth;
	std::string currentScore;
	std::string currentMove;
public:
	Stockfish(std::shared_ptr<chess::Board> board);
	~Stockfish();
	void update();
	void wait_for_ready();
	void new_game();
	void generate_best_move();
	void set_settings(StockfishSettings settings);
	std::string generate_score_string(std::string scoreType, std::string scoreValue);
	bool is_generating_moves();
	chess::Move get_best_move();
	std::string get_search_depth();
	std::string get_score();
	std::string get_current_move();
};
