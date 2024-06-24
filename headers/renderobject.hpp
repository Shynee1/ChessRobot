#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include "utility.hpp"

struct RenderObject {
	SDL_Texture* base_texture = NULL;
	std::unique_ptr<SDL_Rect> src = NULL;
	std::unique_ptr<SDL_Rect> dst = NULL;
	RenderObject(SDL_Texture* tex);
	void set_src(int x, int y, int w, int h);
	void set_dst(int x, int y, int w, int h);
	void set_pos(int x, int y);
	void set_pos(SDL_Point& point);
	void set_dimensions(int w, int h);
	void align_center();
};
