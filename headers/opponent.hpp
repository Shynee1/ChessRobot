#pragma once
#include "utility.hpp"
#include "openingbook.hpp"
#include "stockfish.hpp"
#include "boardui.hpp"
#include "gamemanager.hpp"
#include "motorcontroller.hpp"

class Opponent : public Component {
private:
	const chess::Color color = chess::Color::BLACK;
	const std::array<StockfishSettings, 9> levelMap = { 
		StockfishSettings(1320, 1, 10, 1),
		StockfishSettings(2000, 5, 200, 2),
		StockfishSettings(3190, 22, 1500, 4)
	};
	std::unique_ptr<Stockfish> stockfish;
	std::shared_ptr<GUI> gui;
	std::shared_ptr<chess::Board> board;
	std::shared_ptr<OpeningBook> book;
	std::shared_ptr<MotorController> motorController;
	std::shared_ptr<BoardUI> boardUI;
	int level = 0;
	bool generatedMoves = false;
	chess::Move lastMove = chess::Move::NO_MOVE;
public:
	Opponent() = default;
	void start();
	void update();
	void graphics();
private:
	void play_move(chess::Move& move);
	bool play_book_move();
public:
	std::string get_best_move_text();
	void set_stockfish_level(int level);
};