#include "spritesheet.hpp"

Spritesheet::Spritesheet(std::string path, int numRows, int numCols) {
	m_spritesheet = AssetPool::Instance()->get_texture(path);
	SDL_Point textureDimensions = { 0, 0 };
	SDL_QueryTexture(
		m_spritesheet,
		NULL,
		NULL,
		&textureDimensions.x,
		&textureDimensions.y
	);

	int numSprites = numRows * numCols;
	int spriteWidth = textureDimensions.x / numCols;
	int spriteHeight = textureDimensions.y / numRows;

	int currentX = 0;
	int currentY = 0;

	for (int i = 0; i < numSprites; i++) {
		auto sprite = std::make_shared<RenderObject>(m_spritesheet);

		if (currentX > textureDimensions.x - spriteWidth) {
			currentX = 0;
			currentY += spriteHeight;
		}

		sprite->set_src(
			currentX,
			currentY,
			spriteWidth,
			spriteHeight
		);

		currentX += spriteWidth;
		m_sprites.push_back(sprite);
	}
}

std::shared_ptr<RenderObject> Spritesheet::get_sprite(int index) {
	return m_sprites[index];
} 