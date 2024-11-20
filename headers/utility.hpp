#pragma once
#define WIN32_LEAN_AND_MEAN
#include <array>
#include <vector>
#include <iostream>
#include <memory>
#include <string>
#include <stdint.h>
#include <bitset>
#include <bit>
#include <cmath>
#include <algorithm>
#include <queue>
#include <stack>
#include <fstream>
#include <time.h> 
#include <functional>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <chess.hpp>
#include <serialib.hpp>
#include <fmt/core.h>

typedef std::uint64_t U64;

constexpr int WORLD_WIDTH = 1280;
constexpr int WORLD_HEIGHT = 720;
constexpr int SQUARE_SIZE = WORLD_HEIGHT / 8;
constexpr int BOARD_SHIFT = (WORLD_WIDTH / 2.0) - (WORLD_HEIGHT / 2.0);

constexpr SDL_Color BLACK = {0, 0, 0, 255};
constexpr SDL_Color WHITE = {255, 255, 255, 255};
constexpr SDL_Color RED = {255, 0, 0, 255};
constexpr SDL_Color LIGHT_BLUE = { 66, 135, 245, 255};
constexpr SDL_Color LIGHT_GREY = {236, 236, 236, 255};
constexpr SDL_Color DARK_GREY = {56, 56, 56, 255};
constexpr SDL_Color DARK_RED = {185, 0, 0, 255};

constexpr char STARTING_FEN[] = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

int get_array_index(int rank, int file);
int get_rank(int array_index);
int get_file(int array_index);
int square_distance(chess::Square a, chess::Square b);

int pop_lsb(U64& bitboard);
void set_bit(U64& bitboard, int index);
void clear_bit(U64& bitboard, int index);
void toggle_bit(U64& bitboard, int index);
bool contains_bit(U64& bitboard, int index);
void print_bitboard(U64 bitboard);
U64 fen_to_bitboard(std::string fen);

int get_texture_index(chess::Piece& piece);
int worldToArray(SDL_Point& worldPos);
SDL_Point arrayToWorld(int arrayPos);

std::vector<std::string> split_string(std::string str, char delimiter);
std::string replaceAll(std::string str, std::string what, std::string with);
int signum(int x);
std::vector<std::string> read_all_serial_data(serialib& arduino);

void fatal_log(std::string log);
