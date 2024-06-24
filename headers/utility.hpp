#pragma once
#include <array>
#include <vector>
#include <iostream>
#include <memory>
#include <string>
#include <stdint.h>
#include <bitset>
#include <bit>
#include <cmath>
#include <format>
#include <algorithm>
#include <queue>
#include <stack>
#include <fstream>
#include <chess.hpp>

typedef std::uint64_t U64;

constexpr int WORLD_WIDTH = 1280;
constexpr int WORLD_HEIGHT = 720;

constexpr char STARTING_FEN[] = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

int get_array_index(int rank, int file);
int get_rank(int array_index);
int get_file(int array_index);

int pop_lsb(U64& bitboard);
void set_bit(U64& bitboard, int index);
void clear_bit(U64& bitboard, int index);
void toggle_bit(U64& bitboard, int index);
bool contains_bit(U64& bitboard, int index);
void print_bitboard(U64 bitboard);
U64 fen_to_bitboard(std::string fen);

int get_texture_index(chess::Piece& piece);

std::vector<std::string> split_string(std::string str, char delimiter);
int signum(int x);
