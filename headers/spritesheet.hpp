#pragma once
#include <SDL_image.h>
#include <SDL.h>
#include "utility.hpp"
#include "AssetPool.hpp"

class Spritesheet {
private:
	SDL_Texture* m_spritesheet = NULL;
	std::vector<std::shared_ptr<RenderObject>> m_sprites;

public:
	Spritesheet(std::string filename, int numRows, int numCols);
	std::shared_ptr<RenderObject> get_sprite(int index);
};