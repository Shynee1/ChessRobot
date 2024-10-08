#pragma once
#include "utility.hpp"
#include "renderobject.hpp"
#include "label.hpp"
#include "button.hpp"

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
	bool wasMouseJustPressed = false;
public:
	static std::shared_ptr<Window> Instance();
	Window();
	void create_window(std::string title, int screenWidth, int screenHeight);
private:
	void sdl_init();
	void window_init();
	SDL_Rect world_to_screen(SDL_Rect& rect);
public:
	SDL_Texture* load_texture(const char* filepath);
	TTF_Font* load_font(const char* filepath, int size);
	void render_object(RenderObject *p_obj);
	void render_texture(SDL_Texture *p_texture, int x, int y);
	void render_color(SDL_Rect rect, SDL_Color color);
	void render_label(Label label);
	void render_button(Button& button);
	void display();
	void update();
	void mouse_press_callback(SDL_MouseButtonEvent &e);
	void mouse_unpress_callback(SDL_MouseButtonEvent &e);
	void clear();
	void close();
	bool is_running();
	bool is_mouse_pressed();
	SDL_Point get_mouse_pos();
};
