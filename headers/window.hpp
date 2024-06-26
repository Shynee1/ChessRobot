#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "utility.hpp"
#include "renderobject.hpp"

constexpr SDL_Color BLACK = {0, 0, 0, 255};

class Window {
private:
	static std::shared_ptr<Window> instance;
	
	SDL_Window* window = NULL;
	SDL_Renderer* renderer = NULL;

	std::string title;

	SDL_Point screenDimensions;
	SDL_FPoint screenRatios;

	bool isRunning = false;

	SDL_Point mousePos;
	bool isMousePressed = false;
public:
	static std::shared_ptr<Window> Instance();
	Window();
	void create_window(std::string title, int screenWidth, int screenHeight);
	void sdl_init();
	void window_init();
	SDL_Texture* load_texture(const char* filepath);
	void render_object(RenderObject *p_obj);
	void render_texture(SDL_Texture *p_texture, int x, int y);
	void render_color(SDL_Rect *p_rect, SDL_Color &color);
	void render_text(SDL_Point pos, std::string text, TTF_Font* font, SDL_Color color);
	SDL_Rect world_to_screen(SDL_Rect& rect);
	void display();
	void update(float dt);
	void mouse_press_callback(SDL_MouseButtonEvent &e);
	void mouse_unpress_callback(SDL_MouseButtonEvent &e);
	void clear();
	void close();
	bool is_running();
	bool is_mouse_pressed();
	SDL_Point get_mouse_pos();
};