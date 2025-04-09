#include "boardui.hpp"

using namespace chess;

void BoardUI::start(){
	this->window = Window::Instance();
	this->assetPool = AssetPool::Instance();
	this->board = GameManager::Instance()->get_board();

	TTF_Font* arialLarge = assetPool->get_font("arial80");
	TTF_Font* arialSmall = assetPool->get_font("arial38");
	SDL_Point topPoint = {0, 260};
	SDL_Point bottomPoint = {0, 375};

	this->gameOverText = std::make_unique<Label>(topPoint, "GAME OVER", arialLarge, WHITE);
	this->gameOverText->center_horizontally();

	std::string placeholder = "Placeholder";
	this->championText = std::make_unique<Label>(bottomPoint, placeholder.c_str(), arialSmall, WHITE);
	this->championText->center_horizontally();

	this->spritesheet = std::make_unique<Spritesheet>("chess_pieces.png", 2, 6);
	this->currentBoardState = ~uint64_t(0);

	movegen::legalmoves(moves, *board);
}

void BoardUI::update() {

}

void BoardUI::graphics() {
	SDL_Rect currentSquare = { 0, 0, SQUARE_SIZE, SQUARE_SIZE };

	for (int i = 0; i < 64; i++) {
		Piece piece = board->at(Square(i));

		auto worldPosition = arrayToWorld(i);

		currentSquare.x = worldPosition.x;
		currentSquare.y = worldPosition.y;
		
		SDL_Color& squareColor = coloredSquares[i];
		if (is_visible(squareColor))
			window->render_color(currentSquare, squareColor);
		
		if (piece == Piece::NONE || !contains_bit(currentBoardState, i)) continue;
		
		int textureIndex = get_texture_index(piece);
		auto p_pieceObject = spritesheet->get_sprite(textureIndex);
		p_pieceObject->set_pos(worldPosition);
		p_pieceObject->set_dimensions(SQUARE_SIZE, SQUARE_SIZE);
		window->render_object(p_pieceObject.get());
	}

	auto gameState = board->isGameOver();
    if (gameState.first != chess::GameResultReason::NONE){
		window->render_color(GAME_OVER_RECT, GAME_OVER_COLOR);
		window->render_label(*gameOverText);

		std::string reason = create_text(gameState);
		championText->set_text(reason);
		championText->center_horizontally();
		window->render_label(*championText);
	}
}

std::string BoardUI::create_text(std::pair<chess::GameResultReason, chess::GameResult> gameResult) {
	std::string color = "";
	switch(gameResult.second){
		case chess::GameResult::WIN:
			color = "White won";
			break;
		case chess::GameResult::LOSE:
			color = "Black won";
			break;
		case chess::GameResult::DRAW:
			color = "Game drawn";
			break;
	}

	std::string reason = "";
	switch (gameResult.first){
		case chess::GameResultReason::CHECKMATE:
			reason = "Checkmate";
			break;
		case chess::GameResultReason::FIFTY_MOVE_RULE:
			reason = "50 Move Rule";
			break;
		case chess::GameResultReason::INSUFFICIENT_MATERIAL:
			reason = "Insufficient Material";
			break;
		case chess::GameResultReason::STALEMATE:
			reason = "Stalemate";
			break;
		case chess::GameResultReason::THREEFOLD_REPETITION:
			reason = "Repition";
	}

	return color + " by " + reason;
}

void BoardUI::color_square(int squarePos, const SDL_Color& color) {
	coloredSquares[squarePos] = color;
}

void BoardUI::update_bitboard(U64 bitboard) {
	this->currentBoardState = bitboard;
}

void BoardUI::clear_ui() {
	coloredSquares = {};
}

const SDL_Color& BoardUI::move_color() {
	return MOVE_COLOR;
}

const SDL_Color& BoardUI::clicked_color() {
	return CLICKED_COLOR;
}

bool BoardUI::is_visible(SDL_Color& color) {
	return color.a != 0;
}
