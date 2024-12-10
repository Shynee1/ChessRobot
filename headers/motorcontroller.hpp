#pragma once
#include "utility.hpp"
#include "boardui.hpp"
#include "component.hpp"
#include <regex>

constexpr char ARDUINO_PORT[] = "COM4";
constexpr int BAUD_RATE = 115200;

constexpr float BOARD_OFFSET_X = 61;
constexpr float BOARD_OFFSET_Y = 9; 
constexpr float BOARD_GRADIENT_X = 1;
constexpr float BOARD_GRADIENT_Y = 1;
constexpr float TAKEN_PIECES_X = 0;
constexpr float TAKEN_PIECES_Y = 0;
constexpr float SQUARE_WIDTH = 45.5;
constexpr float PIECE_HEIGHT[7] = { 39.8, 49.8, 61.7, 42.7, 75.1, 82.5, 0 };
constexpr float Z_MAX = 142;
constexpr float Z_MIN = 15;
constexpr float Z_PICKUP_OFFSET = 50;
constexpr int 	DELAY_THRESHOLD = 10;
constexpr int 	PICKUP_DELAY_AMOUNT = 0;

class MotorController : public Component {
private:
	std::shared_ptr<chess::Board> board;
	std::shared_ptr<GUI> gui;  
		
	serialib arduino;
	
	std::queue<std::string> gcodeBuffer;
	bool electroMagnetOn = false;
	bool isWaiting = false;
	bool hasBeenHomed = false;
	int delayTimer = DELAY_THRESHOLD;
	double currentX = 0;
	double currentY = 0;
	double currentZ = 0;
public:
	~MotorController();
	void start();
	void update();
	void graphics();
	void connect();
	bool parse_serial_data(std::vector<std::string> serialData);
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
	void unlock();
	void soft_reset();
	void delay(int milliseconds);
	void poll_machine_state();
	void go_home();
	void wait_for_response();
	float get_z_height(chess::PieceType piece);
};
