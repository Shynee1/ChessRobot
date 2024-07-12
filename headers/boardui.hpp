#pragma once
#include "utility.hpp"
#include "window.hpp"
#include "AssetPool.hpp"
#include "gui.hpp"
#include "spritesheet.hpp"
#include "gamemanager.hpp"

constexpr SDL_Color CLICKED_COLOR = { 255, 217, 0, 60 };
constexpr SDL_Color MOVE_COLOR = { 66, 135, 245, 125 };

class BoardUI : public Component {
private:
	std::unique_ptr<Spritesheet> spritesheet;
	
	std::shared_ptr<AssetPool> assetPool;
	std::shared_ptr<Window> window;
	std::shared_ptr<chess::Board> board;
	std::shared_ptr<GUI> gui;

	chess::Movelist moves;

	std::array<SDL_Color, 64> coloredSquares = {};
public:
	void start();
	void update();
	void graphics();
	void color_square(int squarePos, const SDL_Color& color);
	void clear_ui();
	const SDL_Color& move_color();
	const SDL_Color& clicked_color();
private:
	bool is_visible(SDL_Color& color);
};