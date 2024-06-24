#include "BoardUI.hpp"

using namespace chess;

BoardUI::BoardUI(chess::Board* board, Window* window) {
	this->board = board;
	this->window = window;
	this->spritesheet = std::make_unique<Spritesheet>("chess_pieces.png", 2, 6);
	this->assetPool = AssetPool::Instance();

	SDL_Texture* boardTex = assetPool->get_texture("Board.png");
	this->boardTexture = std::make_unique<RenderObject>(boardTex);
	this->boardTexture->set_dimensions(WORLD_HEIGHT, WORLD_HEIGHT);
	this->boardTexture->align_center();

	movegen::legalmoves(moves, *board);
}

void BoardUI::graphics() {
	window->render(boardTexture.get());

	SDL_Rect currentSquare = { 0, 0, SQUARE_SIZE, SQUARE_SIZE };

	for (int i = 0; i < 64; i++) {
		Piece piece = board->at(Square(i));

		auto worldPosition = arrayToWorld(i);

		currentSquare.x = worldPosition.x;
		currentSquare.y = worldPosition.y;
		SDL_Rect& squareColor = coloredSquares[i];
		if (!SDL_RectEquals(&squareColor, &EMPTY))
			window->render(&currentSquare, coloredSquares[i]);
		
		if (piece == Piece::NONE) continue;
		
		int textureIndex = get_texture_index(piece);
		auto p_pieceObject = spritesheet->get_sprite(textureIndex);
		p_pieceObject->set_pos(worldPosition);
		p_pieceObject->set_dimensions(SQUARE_SIZE, SQUARE_SIZE);
		window->render(p_pieceObject.get());
	}
}

int BoardUI::worldToArray(SDL_Point& worldPos) {
	int rank = (WORLD_HEIGHT - worldPos.y) / SQUARE_SIZE;
	int file = (worldPos.x - BOARD_SHIFT) / SQUARE_SIZE;
	return get_array_index(rank, file);
}

SDL_Point BoardUI::arrayToWorld(int arrayPos) {
	int x = get_file(arrayPos) * SQUARE_SIZE + BOARD_SHIFT;
	int y = (WORLD_HEIGHT - SQUARE_SIZE) - (get_rank(arrayPos) * SQUARE_SIZE);
	return { x, y };
}

void BoardUI::color_square(int squarePos, SDL_Rect& color) {
	coloredSquares[squarePos] = color;
}

void BoardUI::clear_ui() {
	coloredSquares = {};
}

SDL_Rect& BoardUI::move_color() {
	return MOVE_COLOR;
}

SDL_Rect& BoardUI::clicked_color() {
	return CLICKED_COLOR;
}
