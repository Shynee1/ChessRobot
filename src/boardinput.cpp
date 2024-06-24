#include "boardinput.hpp"

using namespace chess;

BoardInput::BoardInput(chess::Board* board, BoardUI* ui) {
	this->board = board;
	this->ui = ui;
	this->previousBoard = fen_to_bitboard(board->getFen());
	movegen::legalmoves(legalMoves, *board);

	char isOpen = serial.openDevice(SERIAL_PORT, SERIAL_BAUD_RATE);
	if (isOpen == 1)
		std::cout << "Connection successfully opened to " << SERIAL_PORT << "\n";
	else
		std::cerr << "ERROR: Could not connect to " << SERIAL_PORT << "\n";
	serial.flushReceiver();


	bitboardData.push(fen_to_bitboard("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/R4BNR w KQkq - 0 1"));
	bitboardData.push(fen_to_bitboard("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/R1K2BNR w KQkq - 0 1"));
}

BoardInput::~BoardInput() {
	serial.closeDevice();
}

void BoardInput::update(float deltaTime) {
	if (board->sideToMove() != Color::WHITE) return;

	U64 currentBoard = read_latest_bitboard();

	std::cout << std::bitset<64>(currentBoard) << "\n";
	/*
	if (currentBoard == previousBoard || currentBoard == 0) return;
	
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
	*/
}

void BoardInput::handle_piece_putdown(int squarePos) {
	if (board->sideToMove() != Color::WHITE 
		|| pickedUpPieces.size() > 2 
		|| pickedUpPieces.size() <= 0) 
			return;

	Move move = NULL;	
	if (pickedUpPieces.size() == 1) 
		move = handle_move(squarePos);
	else {
		int count = std::count(pickedUpPieces.begin(), pickedUpPieces.end(), squarePos);
		if (count > 0) move = handle_capture(squarePos);
		else move = handle_en_passant(squarePos);
	}

	if (move != NULL)
		board->makeMove(move);

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

U64 BoardInput::read_latest_bitboard() {
	U64 bitboard = 0ULL;
	const int numBytes = 8;

	unsigned char buffer[numBytes];
	serial.readBytes(buffer, numBytes);

	for (int i = 0; i < numBytes; i++) {
		bitboard |= (U64) buffer[i] << (i * 8);
	}

	return bitboard;
}

