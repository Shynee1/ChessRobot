#pragma once
#include "utility.hpp"
#include "gamemanager.hpp"
#include "boardui.hpp"
#include <queue>

constexpr int BOARD_BAUD_RATE = 9600;
constexpr char BOARD_ARDUINO_PORT[] = "/dev/ttyACM0";

class BoardInput : public Component {
private:
	std::shared_ptr<chess::Board> board;
	std::shared_ptr<BoardUI> ui;
	serialib serial;

	U64 currentBoard;
	U64 previousBoard;
	chess::Movelist legalMoves;
	std::vector<int> pickedUpPieces;

	std::queue<U64> bitboardData;
public:
	BoardInput() = default;
	~BoardInput();
	void start();
	void update();
	void graphics();
	void connect();
	void handle_piece_putdown(int squarePos);
	void handle_piece_pickup(int squarePos);
	chess::Move handle_move(int squarePos);
	chess::Move handle_capture(int squarePos);
	chess::Move handle_en_passant(int squarePos);
	chess::Move handle_castle(int squarePos);
	chess::Move get_legal_move(int from, int to);
	bool is_castle(int squarePos);
	bool read_latest_bitboard();
	void reset();
};
