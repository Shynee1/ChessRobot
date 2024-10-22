#include "boardinput.hpp"

using namespace chess;

void BoardInput::start() {
	this->board = GameManager::Instance()->get_board();
	this->ui = GameManager::Instance()->get_component<BoardUI>();
	this->currentBoard = fen_to_bitboard(board->getFen());
	this->previousBoard = currentBoard;
	movegen::legalmoves(legalMoves, *board);

	char isOpen = serial.openDevice(SERIAL_PORT, SERIAL_BAUD_RATE);
	if (isOpen == 1)
		std::cout << "Connection successfully opened to " << SERIAL_PORT << "\n";
	else
		std::cerr << "ERROR: Could not connect to " << SERIAL_PORT << "\n";
	serial.flushReceiver();
}

BoardInput::~BoardInput() {
	serial.closeDevice();
}

void BoardInput::update() {
	if (board->sideToMove() != Color::WHITE) return;

	if (!serial.isDeviceOpen()) return;

	bool newData = read_latest_bitboard();
	
	if (!newData) return;

	std::cout << "Current:  " << std::bitset<64>(currentBoard) << std::endl;
	std::cout << "Previous: " << std::bitset<64>(previousBoard) << std::endl;
	
	movegen::legalmoves(legalMoves, *board);

	U64 changedSquares = previousBoard ^ currentBoard;
	while (changedSquares != 0) {
		int squarePos = pop_lsb(changedSquares);

		if (contains_bit(currentBoard, squarePos))
			handle_piece_putdown(squarePos);
		else if (contains_bit(previousBoard, squarePos))
			handle_piece_pickup(squarePos);
		else
			std::cerr << "ERROR: No board contains bit " << squarePos << std::endl;
	}

	previousBoard = currentBoard;
}

void BoardInput::graphics() {

}

void BoardInput::handle_piece_putdown(int squarePos) {
	if (board->sideToMove() != Color::WHITE 
		|| pickedUpPieces.size() > 2 
		|| pickedUpPieces.size() <= 0) {
			pickedUpPieces.clear();
			return;
		}
			

	Move move = NULL;	
	if (pickedUpPieces.size() == 1) 
		move = handle_move(squarePos);
	else {
		int count = std::count(pickedUpPieces.begin(), pickedUpPieces.end(), squarePos);
		if (count > 0) move = handle_capture(squarePos);
		else move = handle_en_passant(squarePos);
	}

	if (move != NULL){
		board->makeMove(move);
		GameManager::Instance()->get_opening_book()->updateMoves(board->hash());
	} 

	pickedUpPieces.clear();

	std::cout << move << "\n";
}

void BoardInput::handle_piece_pickup(int squarePos) {
	if (board->at(squarePos) == chess::Piece::NONE)
		return;

	pickedUpPieces.push_back(squarePos);
}

Move BoardInput::handle_move(int squarePos) {
	if (is_castle(squarePos))
		return handle_castle(squarePos);

	return get_legal_move(pickedUpPieces[0], squarePos);
}

Move BoardInput::handle_capture(int squarePos) {
	for (int i = 0; i < 2; i++) {
		if (pickedUpPieces[i] == squarePos) {
			int source = pickedUpPieces[abs(1 - i)];
			return get_legal_move(source, squarePos);
		}
	}
	return NULL;
}

Move BoardInput::handle_en_passant(int squarePos) {
	for (int i = 0; i < 2; i++) {
		if (get_file(pickedUpPieces[i]) == get_file(squarePos)) {
			int source = pickedUpPieces[abs(1 - i)];
			return get_legal_move(source, squarePos);
		}
	}
	return NULL;
}

bool BoardInput::is_castle(int squarePos) {
	PieceType pieceType = board->at<PieceType>(pickedUpPieces[0]);
	return pieceType == PieceType::KING
		&& abs(pickedUpPieces[0] - squarePos) == 2;
}

Move BoardInput::handle_castle(int squarePos) {
	bool isKingside = squarePos - pickedUpPieces[0] > 0;
	int castleSquareOffset = isKingside ? 1 : -2;
	int target = squarePos + castleSquareOffset;
	return get_legal_move(pickedUpPieces[0], target);
}

Move BoardInput::get_legal_move(int from, int to) {
	for (Move& move : legalMoves) {
		if (move.from().index() == from && move.to().index() == to)
			return move;
	}
	return NULL;
}

bool BoardInput::read_latest_bitboard() {
	const int numBytes = 8;

	if (serial.available() <= 0 )
		return false;

	unsigned char buffer[numBytes];
	serial.readBytes(buffer, numBytes);

	currentBoard = 0ULL;

	for (int i = 0; i < numBytes; i++) {
		currentBoard |= (U64) buffer[i] << (i * 8);
	}

	return true;
}

