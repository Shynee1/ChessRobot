#include "motorcontroller.hpp"

using namespace chess;

MotorController::~MotorController() {
	arduino.closeDevice();
}

void MotorController::start() {
	if (!hasBeenHomed) home_machine();
	this->gui = GUI::Instance();
	this->board = GameManager::Instance()->get_board();
	this->delayTimer = DELAY_THRESHOLD;
}

void MotorController::update() {
	if (!arduino.isDeviceOpen())
		return;
		
	poll_machine_state();
	auto output = read_all_serial_data(arduino);
	isWaiting = parse_serial_data(output);

	if (isWaiting || gcodeBuffer.empty())
		return;
	
	std::string gcodeCommand = gcodeBuffer.front() + "\n";
	std::cout << "Command: " << gcodeCommand; 
	arduino.writeString(gcodeCommand.c_str());
	wait_for_response();
	gcodeBuffer.pop();
}

void MotorController::graphics() {
	gui->get_label("x").set_text(fmt::format("X: {:.3f}", currentX));
	gui->get_label("y").set_text(fmt::format("Y: {:.3f}", currentY));
	gui->get_label("z").set_text(fmt::format("Z: {:.3f}", currentZ));
}

void MotorController::connect() {
	char isOpen = arduino.openDevice(ARDUINO_PORT, BAUD_RATE);
	if (isOpen != 1) return;
	
	GUI::Instance()->get_label("machineConnected").set_text("Connected");

	arduino.writeString("\r\n\r\n");
	arduino.flushReceiver();
	wait_for_response();
}

bool MotorController::parse_serial_data(std::vector<std::string> serialData) {
	bool responseStringFound = false;

	std::regex rgx("<.*MPos:([0-9.-]+),([0-9.-]+),([0-9.-]+).*>");
	std::smatch match;

	for (std::string& s : serialData) {
		if (std::regex_search(s, match, rgx)){
			currentX = std::stod(match[1].str()) + 397;
			currentY = std::stod(match[2].str()) + 350;
			currentZ = std::stod(match[3].str()) + 153;
		}
		else {
			responseStringFound = true;
			std::cout << s;
		}
	}

	return isWaiting ? !responseStringFound : false;
}

void MotorController::send_move(Move move) {
	PieceType sourcePiece = board->at<PieceType>(move.from().index());
	PieceType targetPiece = board->at<PieceType>(move.to().index());

	if (move.typeOf() == Move::CASTLING) {
		int dir = signum(move.to().index() - move.from().index());

		int newRookOffset = dir > 0 ? -2 : 3;
		Square newKingSquare = Square(move.from().index() + dir * 2);
		Square newRookSquare = Square(move.to().index() + newRookOffset);

		move_piece(move.from(), newKingSquare, sourcePiece);
		move_piece(move.to(), newRookSquare, targetPiece, true);
	}
	else if (move.typeOf() == Move::ENPASSANT) {
		int dir = signum(move.to().index() - move.from().index());

		Square captureSquare = Square(move.from().index() + dir);
		targetPiece = board->at<PieceType>(captureSquare);

		capture_piece(captureSquare, targetPiece);
		move_piece(move.from(), move.to(), sourcePiece);
	}
	else if (targetPiece != PieceType::NONE) {
		capture_piece(move.to(), targetPiece);
		move_piece(move.from(), move.to(), sourcePiece);
	}
	else
		move_piece(move.from(), move.to(), sourcePiece);

	go_home();
}

void MotorController::reset() {
	gcodeBuffer.push("G21 G17 G90");
}

void MotorController::set_electromagnet(bool on) {
	if (on)
		gcodeBuffer.push("M03 S1000");
	else 
		gcodeBuffer.push("M05");
}

void MotorController::pickup_piece(PieceType piece, bool fullyLiftPiece) {
	float z = get_z_height(piece);
	move_to(z);
	set_electromagnet(true);
	delay(PICKUP_DELAY_AMOUNT);
	if (fullyLiftPiece) {
		move_to(Z_MIN);
		delay(PICKUP_DELAY_AMOUNT);
	}
}

void MotorController::putdown_piece(PieceType piece, bool isCapturedPiece) {
	float z = get_z_height(piece);
	if (isCapturedPiece) {
		z -= Z_CAPTURED_OFFSET;
	}

	move_to(z);
	set_electromagnet(false);
	move_to(Z_MIN);
}

void MotorController::move_piece(Square from, Square to, PieceType piece, bool castlingOverride) {
	bool fullyLiftPiece = castlingOverride || (piece == PieceType::KNIGHT) || (Square::distance(from, to) > 2);
	move_to_square(from);
	pickup_piece(piece, fullyLiftPiece);
	if (fullyLiftPiece)
		move_to_square(to);
	else
		move_to_square(to, piece);
	putdown_piece(piece, false);
}

void MotorController::capture_piece(chess::Square from, chess::PieceType piece) {
	move_to_square(from);
	pickup_piece(piece, true);
	move_to(takenPiecesX, takenPiecesY);
	putdown_piece(piece, true);

	takenPiecesY -= SQUARE_WIDTH;
	
	if (takenPiecesY < 0) {
		takenPiecesY = TAKEN_PIECES_Y_MAX;
	}
}

void MotorController::move_to_square(Square square) {
	float x = BOARD_OFFSET_X + (square.file() * SQUARE_WIDTH) * BOARD_GRADIENT_X;
	float y = BOARD_OFFSET_Y + ((7 - square.rank()) * SQUARE_WIDTH) * BOARD_GRADIENT_Y;
	move_to(x, y);
}

void MotorController::move_to_square(chess::Square square, chess::PieceType piece) {
	float x = BOARD_OFFSET_X + (square.file() * SQUARE_WIDTH) * BOARD_GRADIENT_X;
	float y = BOARD_OFFSET_Y + ((7 - square.rank()) * SQUARE_WIDTH) * BOARD_GRADIENT_Y;

	float z = get_z_height(piece) - Z_PICKUP_OFFSET;
	move_to(x, y, z);
}

void MotorController::move_to(float x, float y, float z) {
	gcodeBuffer.push(fmt::format("G00 X{} Y{} Z{}", x, y, z));
}

void MotorController::move_to(float x, float y) {
	gcodeBuffer.push(fmt::format("G00 X{} Y{}", x, y));
}

void MotorController::move_to(float z) {
	gcodeBuffer.push(fmt::format("G00 Z{}", z));
}

void MotorController::home_machine() {
	gcodeBuffer.push("$H");
	gcodeBuffer.push("G10 P0 L20 X0 Y0 Z0");
	move_to(BOARD_OFFSET_X, BOARD_OFFSET_Y);
	gcodeBuffer.push("G28.1");
	hasBeenHomed = true;
}

void MotorController::unlock() {
	gcodeBuffer.push("$X");
}

void MotorController::soft_reset() {
	gcodeBuffer.push("^X");
	hasBeenHomed = false;
}

void MotorController::delay(int milliseconds) {
	float seconds = milliseconds / 1000.0f;
    std::string delayCommand = fmt::format("G4 P{}", seconds);
    gcodeBuffer.push(delayCommand);
}

void MotorController::poll_machine_state() {
	if (delayTimer > 0){
		delayTimer -= 1;
		return;
	}

	arduino.writeString("?");
	delayTimer = DELAY_THRESHOLD;
}

void MotorController::go_home() {
	gcodeBuffer.push("G28");
}

void MotorController::wait_for_response() {
	isWaiting = true;
}

float MotorController::get_z_height(PieceType piece) {
	return Z_MAX - fabs(PIECE_HEIGHT[piece] - PIECE_HEIGHT[0]);
}
