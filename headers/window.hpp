#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include "utility.hpp"
#include "renderobject.hpp"

class Window {
private:
	static bool s_isInit;
	SDL_Window *m_window = NULL;
	SDL_Renderer *m_renderer = NULL;
	std::string m_title = "";
	double m_screenWidthRatio = 0;
	double m_screenHeightRatio = 0;
	bool m_isRunning = false;
	SDL_Point m_mousePos;
	bool m_isMousePressed = false;
	const bool m_ava = true; // bc ava is always right!!!!!!
public:
	Window(std::string title, int screenWidth, int screenHeight);
	void sdl_init();
	void create_window(int screenWidth, int screenHeight);
	SDL_Texture* load_texture(const char *p_path);
	void render(RenderObject *p_obj);
	void render(SDL_Texture *p_texture, int x, int y);
	void render(SDL_Rect *p_rect, SDL_Rect &p_color);
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