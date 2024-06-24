#include "renderobject.hpp"

RenderObject::RenderObject(SDL_Texture* tex) {
	base_texture = tex;
	src = std::make_unique<SDL_Rect>();
	dst = std::make_unique<SDL_Rect>();
	SDL_QueryTexture(base_texture, NULL, NULL, &src->w, &src->h);
	dst->w = src->w;
	dst->h = src->h;

}

void RenderObject::align_center() {
	dst->x = (WORLD_WIDTH / 2) - (dst->w / 2);
	dst->y = (WORLD_HEIGHT / 2) - (dst->h / 2);
}

void RenderObject::set_src(int x, int y, int w, int h) {
	src->x = x;
	src->y = y;
	src->w = w;
	src->h = h;
}

void RenderObject::set_dst(int x, int y, int w, int h) {
	dst->x = x;
	dst->y = y;
	dst->w = w;
	dst->h = h;
}

void RenderObject::set_pos(int x, int y) {
	dst->x = x;
	dst->y = y;
}

void RenderObject::set_pos(SDL_Point& point) {
	set_pos(point.x, point.y);
}

void RenderObject::set_dimensions(int w, int h) {
	dst->w = w;
	dst->h = h;
}