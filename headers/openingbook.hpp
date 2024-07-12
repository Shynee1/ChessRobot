#pragma once
#include "utility.hpp"
#include "glaze/glaze.hpp"
#include <algorithm>
#include <filesystem>

struct BookElement {
	U64 zobristKey;
	std::string pgn;
	int from;
	int to;
	bool isCastle;
};

typedef std::vector<std::deque<BookElement>> Book;
constexpr char BOOK_FP[] = "assets\\data\\book.bin";
constexpr char GAMES_FP[] = "assets\\data\\games.txt";
constexpr int MAX_MOVES = 15;

class OpeningBook {
private:
	Book book;
	std::stack<chess::Move> lastMoves;
	std::shared_ptr<chess::Board> board;
public:
	OpeningBook(std::shared_ptr<chess::Board> board);
	void createBook(std::string filepath);
	void loadBook(std::string filepath);
	void updateMoves(U64 newZobrist);
	chess::Move getRandomMove();
	std::deque<BookElement> createMoveQueue(std::string line);
	void unmakeAllMoves();
};