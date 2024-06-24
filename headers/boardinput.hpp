#pragma once
#include "utility.hpp"
#include "serialib.hpp"
#include "boardui.hpp"
#include <queue>

constexpr int SERIAL_BAUD_RATE = 9600;
constexpr char SERIAL_PORT[] = "COM8";

class BoardInput {
private:
	chess::Board* board;
	BoardUI* ui;
	serialib serial;

	U64 previousBoard;
	chess::Movelist legalMoves;
	std::vector<int> pickedUpPieces;
	std::queue<U64> bitboardData;
public:
	BoardInput(chess::Board* board, BoardUI* ui);
	~BoardInput();
	void update(float deltaTime);
	void handle_piece_putdown(int squarePos);
	void handle_piece_pickup(int squarePos);
	chess::Move handle_move(int squarePos);
	chess::Move handle_capture(int squarePos);
	chess::Move handle_en_passant(int squarePos);
	chess::Move handle_castle(int squarePos);
	chess::Move get_legal_move(int from, int to);
	bool is_castle(int squarePos);
	U64 read_latest_bitboard();
};