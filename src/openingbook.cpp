#include "openingbook.hpp"

using namespace chess;

OpeningBook::OpeningBook(Board* board) {
	this->board = board;
	if (std::filesystem::exists(BOOK_FP))
		loadBook(BOOK_FP);
	else
		createBook(GAMES_FP);
}

void OpeningBook::createBook(std::string filepath) {
	std::ifstream gamesFile(filepath, std::ifstream::in);

	std::string line;
	while (std::getline(gamesFile, line)) {
		auto moveQueue = createMoveQueue(line);
		book.push_back(moveQueue);
	}

	glz::write_file_binary(book, BOOK_FP, std::string{});

	gamesFile.close();
}

void OpeningBook::loadBook(std::string filepath) {
	glz::read_file_binary(book, BOOK_FP, std::string{});
}

void OpeningBook::updateMoves(U64 newZobrist) {
	for (int i = book.size() - 1; i >= 0; i--) {
		auto& opening = book[i];
		if (opening.empty() || opening.front().zobristKey != newZobrist)
			book.erase(book.begin() + i);
		else
			opening.pop_front();
	}
}

chess::Move OpeningBook::getRandomMove() {
	if (book.size() == 0) return Move::NO_MOVE;

	bool isOneElement = book.size() == 1;
	int pos = isOneElement ? 0 : (int)(rand() % book.size());

	if (book[pos].empty()) return Move::NO_MOVE;

	BookElement element = book[pos].front();
	book[pos].pop_front();

	if (element.isCastle)
		return Move::make<Move::CASTLING>(Square(element.from), Square(element.to));
	else
		return Move::make<Move::NORMAL>(Square(element.from), Square(element.to));
}

std::deque<BookElement> OpeningBook::createMoveQueue(std::string line) {
	std::deque<BookElement> moveQueue;
	auto moves = split_string(line, ' ');
	
	int queueSize = std::min(MAX_MOVES, (int) moves.size());
	for (int i = 0; i < queueSize; i++) {
		std::string pgn = moves[i];

		Move move = uci::parseSan(*board, pgn);

		board->makeMove(move);

		lastMoves.push(move);

		moveQueue.emplace_back(
			board->hash(), 
			pgn, 
			move.from().index(), 
			move.to().index(), 
			move.typeOf() == Move::CASTLING
		);
	}

	unmakeAllMoves();

	return moveQueue;
}

void OpeningBook::unmakeAllMoves() {
	while (!lastMoves.empty()) {
		Move lastMove = lastMoves.top();
		board->unmakeMove(lastMove);
		lastMoves.pop();
	}
}
