#pragma once
#include "utility.hpp"
#include "serialib.hpp"
#include "chess.hpp"

constexpr char ARDUINO_PORT[] = "COM4";
constexpr int BAUD_RATE = 115200;

constexpr float BOARD_OFFSET_X = 68;
constexpr float BOARD_OFFSET_Y = 5; 
constexpr float TAKEN_PIECES_X = 0;
constexpr float TAKEN_PIECES_Y = 170;
constexpr float SQUARE_WIDTH = 45;
constexpr float PIECE_HEIGHT[7] = { 39.8, 51.8, 61.7, 42.7, 76.1, 83.5, 0 };
constexpr float Z_MAX = 103;
constexpr float Z_MIN = 10;
constexpr float Z_PICKUP_OFFSET = 10;

class MotorController {
private:
	chess::Board* board;
	serialib arduino;
	std::queue<std::string> gcodeBuffer;
	bool electroMagnetOn = false;
	bool isWaiting = false;
	int delayTimer = 0;
public:
	MotorController(chess::Board* board);
	~MotorController();
	void update(float dt);
	bool should_wait();
	void send_move(chess::Move move);
	void reset();
	void set_electromagnet(bool on);
	void pickup_piece(chess::PieceType piece, bool fullyLiftPiece);
	void putdown_piece(chess::PieceType piece);
	void move_piece(chess::Square from, chess::Square to, chess::PieceType piece, bool castlingOverride = false);
	void capture_piece(chess::Square from, chess::PieceType piece);
	void move_to_square(chess::Square square);
	void move_to_square(chess::Square square, chess::PieceType piece);
	void move_to(float x, float y, float z);
	void move_to(float x, float y);
	void move_to(float z);
	void home_machine();
	void go_home();
	void wait_for_response(int postResponseDelay);
	float get_z_height(chess::PieceType piece);
};