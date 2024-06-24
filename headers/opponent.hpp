#pragma once
#include "utility.hpp"
#include "stockfish.hpp"
#include "boardui.hpp"
#include "motorcontroller.hpp"
#include "openingbook.hpp"

class Opponent {
private:
	const chess::Color color = chess::Color::BLACK;
	const std::array<StockfishSettings, 9> levelMap = { 
		StockfishSettings(-20, 1, 10, 2),
		StockfishSettings(-9, 5, 50, 2),
		StockfishSettings(-5, 5, 100, 2),
		StockfishSettings(-1, 5, 150, 2),
		StockfishSettings(3, 5, 200, 2),
		StockfishSettings(7, 5, 300, 2),
		StockfishSettings(11, 8, 400, 2),
		StockfishSettings(7, 16, 500, 2),
		StockfishSettings(20, 22, 1500, 2)
	};
	std::unique_ptr<Stockfish> stockfish;
	chess::Board* board;
	BoardUI* ui;
	OpeningBook* book;
	MotorController* motorController;
	int level;
	bool generatedMoves = false;
public:
	Opponent(chess::Board* board, BoardUI* ui, OpeningBook* book, MotorController* mc, int level);
	void update(float deltaTime);
	void play_move(chess::Move move);
	bool play_book_move();
};