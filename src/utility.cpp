#include "utility.hpp"

int get_array_index(int rank, int file) {
	return rank * 8 + file;
}

int get_rank(int array_index) {
	return array_index / 8;
}

int get_file(int array_index) {
	return array_index - get_rank(array_index) * 8;
}

int pop_lsb(U64& bitboard) {
	int index = std::countr_zero(bitboard);
	bitboard &= (bitboard - 1);
	return index;
}

void set_bit(U64& bitboard, int index) {
	bitboard |= 1ULL << index;
}

void clear_bit(U64& bitboard, int index) {
	bitboard &= ~(1ULL << index);
}

void toggle_bit(U64& bitboard, int index) {
	bitboard ^= 1ULL << index;
}

bool contains_bit(U64& bitboard, int index) {
	return ((bitboard >> index) & 1) == 1;
}

void print_bitboard(U64 bitboard) {
	for (int col = 7; col >= 0; col--) {
		for (int row = 0; row < 8; row++) {
			std::cout << contains_bit(bitboard, get_array_index(col, row)) << " ";
		}
		std::cout << "\n";
	}
}

U64 fen_to_bitboard(std::string fen) {
	U64 bitboard = 0ULL;
	auto boardData = split_string(fen, ' ')[0];
	int square = 56;
	for (char c : boardData) {
		if (isdigit(c)) {
			square += static_cast<int>(c - '0');
		}
		else if (isalpha(c)) {
			bitboard |= 1ULL << square;
			square++;
		}
		else {
			square -= 16;
		}
	}
	return bitboard;
}


int get_texture_index(chess::Piece& piece) {
	bool isWhite = piece.color() == chess::Color::WHITE;
	int colorIndex = isWhite ? 0 : 6;

	switch (piece.type().internal()) {
	case chess::PieceType::PAWN:
		return colorIndex + 5;
	case chess::PieceType::KNIGHT:
		return colorIndex + 3;
	case chess::PieceType::BISHOP:
		return colorIndex + 2;
	case chess::PieceType::ROOK:
		return colorIndex + 4;
	case chess::PieceType::QUEEN:
		return colorIndex + 1;
	case chess::PieceType::KING:
		return colorIndex;
	}

	return colorIndex;
}

std::vector<std::string> split_string(std::string str, char delimiter) {
	std::vector<std::string> res;

	int start = 0;
	int end = 0;
	while (end != -1) {
		end = static_cast<int>(str.find(delimiter, start));
		res.push_back(str.substr(start, (size_t)end - start));
		start = end + 1;
	}

	return res;
}

int signum(int x) {
	if (x > 0)
		return 1;
	else if (x < 0)
		return -1;
	else
		return 0;
}

