#pragma once
#include "utility.hpp"
#include "gamemanager.hpp"
#include "boardui.hpp"
#include <queue>
#include <chrono>
#include <ctime>

constexpr int BOARD_BAUD_RATE = 9600;
constexpr char BOARD_ARDUINO_PORT[] = "/dev/ttyACM0";
constexpr unsigned char START_MARKER = 0xAA;
constexpr unsigned char END_MARKER = 0x55;
constexpr int BOARD_DATA_SIZE = 8;

enum class ReadState {
	WAITING_FOR_START,
	READING_DATA,
	COMPLETE_FRAME
};

class BoardInput : public Component {
private:
	std::shared_ptr<chess::Board> board;
	std::shared_ptr<BoardUI> ui;
	serialib serial;

	U64 startBitboard;
	U64 currentBoard;
	U64 previousBoard;
	chess::Movelist legalMoves;
	std::vector<int> pickedUpPieces;

	std::queue<U64> bitboardData;
	
    ReadState serialReadState = ReadState::WAITING_FOR_START;
    unsigned char dataBuffer[BOARD_DATA_SIZE];
    int dataBufferIndex = 0;
    std::chrono::steady_clock::time_point lastFrameTime;
    static const int FRAME_TIMEOUT_MS = 500;
public:
	BoardInput() = default;
	~BoardInput();
	void start();
	void update();
	void graphics();
	void connect();
	void handle_piece_putdown(int squarePos);
	void handle_piece_pickup(int squarePos);
	chess::Move handle_castle(int fromSquare, int toSquare);
	chess::Move get_legal_move(int from, int to);
	bool is_castle(int fromSquare, int toSquare);
	bool read_latest_bitboard();
	void reset();
};
