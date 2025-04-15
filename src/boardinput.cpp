#include "boardinput.hpp"

using namespace chess;

void BoardInput::start() {
	this->board = GameManager::Instance()->get_board();
	this->ui = GameManager::Instance()->get_component<BoardUI>();
	this->startBitboard = fen_to_bitboard(board->getFen());
	this->currentBoard = startBitboard;
	this->previousBoard = currentBoard;
	movegen::legalmoves(legalMoves, *board);

	serial.flushReceiver();
}

BoardInput::~BoardInput() {
	serial.closeDevice();
}

void BoardInput::update() {
	if (board->sideToMove() != Color::WHITE || !serial.isDeviceOpen()) return;

	bool newData = read_latest_bitboard();
	
	if (!newData) return;

	// Clear array after board "calibration"
	if (currentBoard == startBitboard)
		pickedUpPieces.clear();

	this->ui->update_bitboard(currentBoard);
	
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

void BoardInput::connect() {
	char isOpen = serial.openDevice(BOARD_ARDUINO_PORT, BOARD_BAUD_RATE);
	if (isOpen != 1) return;
	
	GUI::Instance()->get_label("boardConnected").set_text("Connected");

	serial.flushReceiver();
}

void BoardInput::handle_piece_putdown(int squarePos) {
	Move move = NULL;	

	for (int fromSquare : pickedUpPieces) {
		if (is_castle(fromSquare, squarePos))
			move = handle_castle(fromSquare, squarePos);
		else
			move = get_legal_move(fromSquare, squarePos);
			
		if (move != NULL) break;
	}

	if (move == NULL)
		return;
	
	pickedUpPieces.clear();

	board->makeMove(move);
	GameManager::Instance()->get_opening_book()->updateMoves(board->hash());

	std::cout << move << "\n";
}

void BoardInput::handle_piece_pickup(int squarePos) {
	if (board->at(squarePos) == chess::Piece::NONE)
		return;

	pickedUpPieces.push_back(squarePos);
}

bool BoardInput::is_castle(int fromSquare, int toSquare) {
	PieceType pieceType = board->at<PieceType>(fromSquare);
	return pieceType == PieceType::KING
		&& abs(fromSquare - toSquare) == 2;
}

Move BoardInput::handle_castle(int fromSquare, int toSquare) {
	bool isKingside = toSquare - fromSquare > 0;
	int castleSquareOffset = isKingside ? 1 : -2;
	int target = toSquare + castleSquareOffset;
	return get_legal_move(fromSquare, target);
}

Move BoardInput::get_legal_move(int from, int to) {
	for (Move& move : legalMoves) {
		if (move.from().index() == from && move.to().index() == to)
			return move;
	}
	return NULL;
}

void BoardInput::reset() {
	this->previousBoard = currentBoard;
}

bool BoardInput::read_latest_bitboard() {
    auto currentTime = std::chrono::steady_clock::now();
    
    // Check for timeout on partial frames
    if (serialReadState != ReadState::WAITING_FOR_START) {
        auto elapsedMs = std::chrono::duration_cast<std::chrono::milliseconds>(
            currentTime - lastFrameTime).count();
            
        if (elapsedMs > FRAME_TIMEOUT_MS) {
            std::cerr << "Frame timeout - resetting state machine" << std::endl;
            serialReadState = ReadState::WAITING_FOR_START;
            dataBufferIndex = 0;
        }
    }
    
    while (serial.available() > 0) {
        unsigned char inByte;
		if (serialReadState != ReadState::READING_DATA && !serial.readBytes(&inByte, 1))
			continue;
        
        switch (serialReadState) {
            case ReadState::WAITING_FOR_START:
			{
                if (inByte == START_MARKER) {
                    serialReadState = ReadState::READING_DATA;
                    dataBufferIndex = 0;
                    lastFrameTime = currentTime;
                }
			}
            break;
                
            case ReadState::READING_DATA:
			{
				int bytesAvailable = serial.available();
				int maxBytes = BOARD_DATA_SIZE - dataBufferIndex;

				int bytesToRead = std::min(bytesAvailable, maxBytes);

				serial.readBytes(&dataBuffer[dataBufferIndex], bytesToRead);
				dataBufferIndex += bytesToRead;
				
				lastFrameTime = currentTime;
                
                // Check if we've read all data bytes
                if (dataBufferIndex == BOARD_DATA_SIZE) {
                    serialReadState = ReadState::COMPLETE_FRAME;
                }
			}
            break;

            case ReadState::COMPLETE_FRAME:
			{
                // We expect this byte to be the end marker
                if (inByte == END_MARKER) {
                    currentBoard = 0ULL;
                    for (int i = 0; i < BOARD_DATA_SIZE; i++) {
                        currentBoard |= (U64) dataBuffer[i] << (i * 8);
                    }
                    
                    serialReadState = ReadState::WAITING_FOR_START;
                    dataBufferIndex = 0;
                    
                    return true;
                } else {
                    std::cerr << "Invalid end marker: 0x" << std::hex 
                              << static_cast<int>(inByte) << std::dec << std::endl;
                    
                    serialReadState = ReadState::WAITING_FOR_START;
                    dataBufferIndex = 0;
                }
			}
            break;
        }
    }

    return false;
}