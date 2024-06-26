#pragma once

#include "utility.hpp"
#include "window.hpp"
#include "AssetPool.hpp"
#include "spritesheet.hpp"

constexpr int SQUARE_SIZE = WORLD_HEIGHT / 8;
constexpr int BOARD_SHIFT = (WORLD_WIDTH / 2.0) - (WORLD_HEIGHT / 2.0);

class BoardUI {
private:
	SDL_Color CLICKED_COLOR = { 255, 217, 0, 60 };
	SDL_Color MOVE_COLOR = { 66, 135, 245, 125 };

	std::unique_ptr<Spritesheet> spritesheet;
	std::unique_ptr<RenderObject> boardTexture;
	std::shared_ptr<AssetPool> assetPool;
	std::shared_ptr<Window> window;

	chess::Board* board;
	chess::Movelist moves;

	std::array<SDL_Color, 64> coloredSquares = {};
public:
	BoardUI(chess::Board* board);
	void graphics();
	int worldToArray(SDL_Point& worldPos);
	SDL_Point arrayToWorld(int arrayPos);
	void color_square(int squarePos, SDL_Color& color);
	void clear_ui();
	SDL_Color& move_color();
	SDL_Color& clicked_color();
	bool is_visible(SDL_Color& color);
};