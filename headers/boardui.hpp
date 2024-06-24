#pragma once

#include "utility.hpp"
#include "window.hpp"
#include "AssetPool.hpp"
#include "spritesheet.hpp"

constexpr int SQUARE_SIZE = WORLD_HEIGHT / 8;
constexpr int BOARD_SHIFT = (WORLD_WIDTH / 2.0) - (WORLD_HEIGHT / 2.0);

class BoardUI {
private:
	SDL_Rect CLICKED_COLOR = { 255, 217, 0, 60 };
	SDL_Rect MOVE_COLOR = { 66, 135, 245, 125 };
	SDL_Rect EMPTY = { 0, 0, 0, 0 };

	std::unique_ptr<Spritesheet> spritesheet;
	std::unique_ptr<RenderObject> boardTexture;
	std::shared_ptr<AssetPool> assetPool;

	chess::Board* board;
	chess::Movelist moves;

	Window* window;

	std::array<SDL_Rect, 64> coloredSquares = {};
public:
	BoardUI(chess::Board* board, Window* window);
	void graphics();
	int worldToArray(SDL_Point& worldPos);
	SDL_Point arrayToWorld(int arrayPos);
	void color_square(int squarePos, SDL_Rect& color);
	void clear_ui();
	SDL_Rect& move_color();
	SDL_Rect& clicked_color();
};