#include "window.hpp"

bool Window::s_isInit = false;

Window::Window(std::string title, int screenWidth, int screenHeight) {
	m_screenWidthRatio = (double) screenWidth / WORLD_WIDTH;
	m_screenHeightRatio = (double) screenHeight / WORLD_HEIGHT;
	m_title = title;
	m_isRunning = true;

	try {
		if (!Window::s_isInit) sdl_init();
		create_window(screenWidth, screenHeight);
	}
	catch (std::exception& e) {
		std::cerr << e.what() << "\n";
		exit(EXIT_FAILURE);
	}
}

void Window::sdl_init() {
	if (SDL_Init(SDL_INIT_VIDEO) > 0)
		throw std::runtime_error("SDL failed to initialize: " + std::string(SDL_GetError()));

	if (!(IMG_Init(IMG_INIT_PNG)))
		throw std::runtime_error("SDL failed to initialize images: " + std::string(IMG_GetError()));

	Window::s_isInit = true;
}

void Window::create_window(int screenWidth, int screenHeight) {
	m_window = SDL_CreateWindow(
		m_title.c_str(),
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		screenWidth,
		screenHeight,
		SDL_WINDOW_SHOWN
	);

	if (m_window == NULL)
		throw std::runtime_error("SDL failed to create window: " + std::string(SDL_GetError()));

	m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED);

	if (m_renderer == NULL)
		throw std::runtime_error("SDL failed to create renderer: " + std::string(SDL_GetError()));

	SDL_SetRenderDrawColor(m_renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
	SDL_SetRenderDrawBlendMode(m_renderer, SDL_BLENDMODE_BLEND);
}

SDL_Texture* Window::load_texture(const char *p_path) {
	SDL_Texture *p_texture = NULL;
	p_texture = IMG_LoadTexture(m_renderer, p_path);

	if (p_texture == NULL)
		std::cerr << "Failed to create texture at " << p_path << "\n" << std::string(SDL_GetError()) << "\n";

	return p_texture;
}

void Window::render(RenderObject* p_obj) {
	p_obj->dst->x *= m_screenWidthRatio;
	p_obj->dst->y *= m_screenHeightRatio;
	p_obj->dst->w *= m_screenWidthRatio;
	p_obj->dst->h *= m_screenHeightRatio;

	SDL_RenderCopy(m_renderer, p_obj->base_texture, p_obj->src.get(), p_obj->dst.get());
}

void Window::render(SDL_Texture *p_texture, int x, int y) {
	SDL_Rect dst;
	SDL_QueryTexture(p_texture, NULL, NULL, &dst.w, &dst.h);
	dst.x = x * m_screenWidthRatio;
	dst.y = y * m_screenHeightRatio;
	dst.w = dst.w * m_screenWidthRatio;
	dst.h = dst.h * m_screenHeightRatio;
	SDL_RenderCopy(m_renderer, p_texture, NULL, &dst);
}

void Window::render(SDL_Rect *p_rect, SDL_Rect &p_color) {
	SDL_SetRenderDrawColor(
		m_renderer, 
		p_color.x, 
		p_color.y, 
		p_color.w, 
		p_color.h
	);
	SDL_RenderFillRect(m_renderer, p_rect);
	SDL_SetRenderDrawColor(m_renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
}

void Window::update(float dt) {
	int mouseX = 0;
	int mouseY = 0;
	SDL_GetMouseState(&mouseX, &mouseY);

	m_mousePos.x = mouseX * m_screenWidthRatio;
	m_mousePos.y = mouseY * m_screenHeightRatio;
}

void Window::mouse_press_callback(SDL_MouseButtonEvent &e) {
	if (e.button != SDL_BUTTON_LEFT) return;
	m_isMousePressed = true;
}


void Window::mouse_unpress_callback(SDL_MouseButtonEvent &e) {
	if (e.button != SDL_BUTTON_LEFT) return;
	m_isMousePressed = false;
}


void Window::close() {
	m_isRunning = false;
	SDL_DestroyWindow(m_window);
	SDL_DestroyRenderer(m_renderer);
	m_window = NULL;
	m_renderer = NULL;
	IMG_Quit();
	SDL_Quit();
}

void Window::display() {
	SDL_RenderPresent(m_renderer);
}

void Window::clear() {
	SDL_RenderClear(m_renderer);
}

bool Window::is_running() {
	return m_isRunning;
}

bool Window::is_mouse_pressed() {
	return m_isMousePressed;
}

SDL_Point Window::get_mouse_pos() {
	return m_mousePos;
}