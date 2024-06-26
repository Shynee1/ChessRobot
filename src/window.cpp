#include "window.hpp"

std::shared_ptr<Window> Window::instance = NULL;

std::shared_ptr<Window> Window::Instance() {
	if (Window::instance == NULL) 
		Window::instance = std::make_shared<Window>();
	
	return Window::instance;
}

Window::Window(){
	this->title = "Window";
	this->screenDimensions = {WORLD_WIDTH, WORLD_HEIGHT};
	this->screenRatios = {1, 1};
}

void Window::create_window(std::string title, int screenWidth, int screenHeight) {
	this->title = title;
	this->screenDimensions = {screenWidth, screenHeight};
	this->screenRatios.x = (double) screenWidth / WORLD_WIDTH;
	this->screenRatios.y = (double) screenHeight / WORLD_HEIGHT;
	this->isRunning = true;

	try {
		sdl_init();
		window_init();
	}
	catch (std::exception& e) {
		std::cerr << e.what() << "\n";
		exit(EXIT_FAILURE);
	}
}

void Window::sdl_init() {
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
		throw std::runtime_error("SDL failed to initialize: " + std::string(SDL_GetError()));

	if (IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG)
		throw std::runtime_error("SDL failed to initialize images: " + std::string(IMG_GetError()));

	if (TTF_Init() < 0) {
		throw std::runtime_error("SDL failed to initialize ttf: " + std::string(TTF_GetError()));
}
}

void Window::window_init() {
	this->window = SDL_CreateWindow(
		title.c_str(),
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		screenDimensions.x,
		screenDimensions.y,
		SDL_WINDOW_SHOWN
	);

	if (window == NULL)
		throw std::runtime_error("SDL failed to create window: " + std::string(SDL_GetError()));

	this->renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	if (renderer == NULL)
		throw std::runtime_error("SDL failed to create renderer: " + std::string(SDL_GetError()));

	SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
}

SDL_Texture* Window::load_texture(const char* filepath) {
	SDL_Texture *p_texture = NULL;
	p_texture = IMG_LoadTexture(renderer, filepath);

	if (p_texture == NULL)
		std::cerr << "Failed to create texture at " << filepath << "\n" << std::string(SDL_GetError()) << "\n";

	return p_texture;
}

void Window::render_object(RenderObject* p_obj) {
	SDL_Rect resized = world_to_screen(*p_obj->dst);
	SDL_RenderCopy(renderer, p_obj->base_texture, p_obj->src.get(), &resized);
}

void Window::render_texture(SDL_Texture *p_texture, int x, int y) {
	SDL_Rect dst;
	SDL_QueryTexture(p_texture, NULL, NULL, &dst.w, &dst.h);
	dst.x = x;
	dst.y = y;
	SDL_Rect resized = world_to_screen(dst);
	SDL_RenderCopy(renderer, p_texture, NULL, &resized);
}

void Window::render_color(SDL_Rect *p_rect, SDL_Color &color) {
	SDL_Rect resized = world_to_screen(*p_rect);
	SDL_SetRenderDrawColor(
		renderer, 
		color.r, 
		color.g, 
		color.b, 
		color.a
	);
	SDL_RenderFillRect(renderer, &resized);
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
}

void Window::render_text(SDL_Point pos, std::string text, TTF_Font* font, SDL_Color color) {
	SDL_Surface* textSurface = TTF_RenderText_Blended(font, text.c_str(), color);
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, textSurface);

	SDL_Rect dst;
	dst.x = pos.x;
	dst.y = pos.y;
	dst.w = textSurface->w;
	dst.h = textSurface->h;

	SDL_Rect resized = world_to_screen(dst);

	SDL_RenderCopy(renderer, texture, NULL, &resized);
	SDL_FreeSurface(textSurface);
	SDL_DestroyTexture(texture);
}

SDL_Rect Window::world_to_screen(SDL_Rect& rect) {
	SDL_Rect converted;
	converted.x = rect.x * screenRatios.x;
	converted.y = rect.y * screenRatios.y;
	converted.w = rect.w * screenRatios.x;
	converted.h = rect.h * screenRatios.y;
	return converted;
}

void Window::update(float dt) {
	int mouseX = 0;
	int mouseY = 0;
	SDL_GetMouseState(&mouseX, &mouseY);

	this->mousePos.x = mouseX * (1/screenRatios.x);
	this->mousePos.y = mouseY * (1/screenRatios.y);
}

void Window::mouse_press_callback(SDL_MouseButtonEvent &e) {
	if (e.button != SDL_BUTTON_LEFT) return;
	isMousePressed = true;
}


void Window::mouse_unpress_callback(SDL_MouseButtonEvent &e) {
	if (e.button != SDL_BUTTON_LEFT) return;
	isMousePressed = false;
}


void Window::close() {
	isRunning = false;
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	this->window = NULL;
	this->renderer = NULL;
	IMG_Quit();
	SDL_Quit();
}

void Window::display() {
	SDL_RenderPresent(renderer);
}

void Window::clear() {
	SDL_RenderClear(renderer);
}

bool Window::is_running() {
	return this->isRunning;
}

bool Window::is_mouse_pressed() {;
	return this->isMousePressed;
}

SDL_Point Window::get_mouse_pos() {
	return this->mousePos;
}