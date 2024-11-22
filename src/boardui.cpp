#include "boardui.hpp"

using namespace chess;

void BoardUI::start(){
	this->window = Window::Instance();
	this->assetPool = AssetPool::Instance();
	this->gui = GUI::Instance();
	this->board = GameManager::Instance()->get_board();

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
