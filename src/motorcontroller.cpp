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
	isWaiting = should_wait();

	if (isWaiting || gcodeBuffer.empty() || !arduino.isDeviceOpen())
		return;

	std::string gcodeCommand = gcodeBuffer.front() + "\n";
	bool isDelay = toupper(gcodeCommand[0]) == 'D';
	if (isDelay) {
		int delayAmount = stoi(gcodeCommand.substr(2));
		wait_for_response(delayAmount);
	}
	else {
		arduino.writeString(gcodeCommand.c_str());
		wait_for_response(0);
	}

	gcodeBuffer.pop();
}

void MotorController::graphics() {
	if (!arduino.isDeviceOpen())
		return;
	
	if (delayTimer > 0){
		delayTimer--;
		return;
	}

	arduino.writeString("?");

	char buffer[100];
	arduino.readString(buffer, '>', 100);

	std::string machineData = std::string(buffer);
	std::string positionData = split_string(machineData, '|')[1];
	auto betterPositionData = split_string(positionData.substr(5), ',');

	double x = stod(betterPositionData[0]) + 399.0;
	double y = stod(betterPositionData[1]) + 349.0;
	double z = stod(betterPositionData[2]) + 104.0;

	GUI::Instance()->get_label("x").set_text(std::format("X: {0:.2f}", x));
	GUI::Instance()->get_label("y").set_text(std::format("Y: {0:.2f}", y));
	GUI::Instance()->get_label("z").set_text(std::format("Z: {0:.2f}", z));

	delayTimer = 100;	
}

void MotorController::connect() {
	char isOpen = arduino.openDevice(ARDUINO_PORT, BAUD_RATE);
	if (isOpen != 1) return;
	
	GUI::Instance()->get_label("machineConnected").set_text("Connected");

	arduino.writeString("\r\n\r\n");
	arduino.flushReceiver();
	wait_for_response(0);
}

bool MotorController::should_wait() {
	if (!isWaiting) return false;

	if (delayTimer > 0) {
		delayTimer -= 1;
		return true;
	}

	if (arduino.available() <= 0) return true;
		
	char buffer[1000];
	arduino.readString(buffer, '\r', 1000);
	std::cout << buffer << "\n";

	arduino.flushReceiver();

	return false;
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
	if (fullyLiftPiece) 
		move_to(Z_MIN);
}

void MotorController::putdown_piece(PieceType piece) {
	float z = get_z_height(piece);
	move_to(z);
	set_electromagnet(false);
	move_to(Z_MIN);
}

void MotorController::move_piece(Square from, Square to, PieceType piece, bool castlingOverride) {
	bool fullyLiftPiece = castlingOverride || (piece == PieceType::KNIGHT);
	move_to_square(from);
	pickup_piece(piece, fullyLiftPiece);
	if (fullyLiftPiece)
		move_to_square(to);
	else
		move_to_square(to, piece);
	putdown_piece(piece);
}

void MotorController::capture_piece(chess::Square from, chess::PieceType piece) {
	move_to_square(from);
	pickup_piece(piece, true);
	move_to(TAKEN_PIECES_X, TAKEN_PIECES_Y);
	putdown_piece(piece);
}

void MotorController::move_to_square(Square square) {
	float x = BOARD_OFFSET_X + (square.file() * SQUARE_WIDTH);
	float y = BOARD_OFFSET_Y + ((7 - square.rank()) * SQUARE_WIDTH);
	move_to(x, y);
}

void MotorController::move_to_square(chess::Square square, chess::PieceType piece) {
	float x = BOARD_OFFSET_X + (square.file() * SQUARE_WIDTH);
	float y = BOARD_OFFSET_Y + ((7 - square.rank()) * SQUARE_WIDTH);
	float z = get_z_height(piece) - Z_PICKUP_OFFSET;
	move_to(x, y, z);
}

void MotorController::move_to(float x, float y, float z) {
	gcodeBuffer.push(std::format("G00 X{} Y{} Z{}", x, y, z));
}

void MotorController::move_to(float x, float y) {
	gcodeBuffer.push(std::format("G00 X{} Y{}", x, y));
}

void MotorController::move_to(float z) {
	gcodeBuffer.push(std::format("G00 Z{}", z));
}

void MotorController::home_machine() {
	gcodeBuffer.push("$H");
	gcodeBuffer.push("G10 P0 L20 X0 Y0 Z0");
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

void MotorController::go_home() {
	gcodeBuffer.push("G28");
}

void MotorController::wait_for_response(int postResponseDelay) {
	isWaiting = true;
	delayTimer = postResponseDelay;
}

float MotorController::get_z_height(PieceType piece) {
	return Z_MAX - fabs(PIECE_HEIGHT[piece] - PIECE_HEIGHT[0]);
}