#pragma once
#include "utility.hpp"
#include "window.hpp"
#include "assetpool.hpp"
#include "gui.hpp"
#include "spritesheet.hpp"
#include "gamemanager.hpp"

constexpr SDL_Rect GAME_OVER_RECT = {325, 135, 630, 450};
constexpr SDL_Color CLICKED_COLOR = {255, 217, 0, 60};
constexpr SDL_Color MOVE_COLOR = {66, 135, 245, 125};
constexpr SDL_Color GAME_OVER_COLOR = {0, 0, 0, 220};

class BoardUI : public Component {
private:
	std::unique_ptr<Spritesheet> spritesheet;
	std::unique_ptr<Label> gameOverText;
	std::unique_ptr<Label> championText;
	
	std::shared_ptr<AssetPool> assetPool;
	std::shared_ptr<Window> window;
	std::shared_ptr<chess::Board> board;

	U64 currentBoardState;
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
	void update_bitboard(U64 bitboard);
private:
	bool is_visible(SDL_Color& color);
	std::string create_text(std::pair<chess::GameResultReason, chess::GameResult> gameResult);
};
