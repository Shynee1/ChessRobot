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
	Label::set_screen_ratios(screenRatios);
}

void Window::create_window(std::string title, int screenWidth, int screenHeight) {
	this->title = title;
	this->screenDimensions = {screenWidth, screenHeight};
	this->screenRatios.x = (double) screenDimensions.x / WORLD_WIDTH;
	this->screenRatios.y = (double) screenDimensions.y / WORLD_HEIGHT;
	Label::set_screen_ratios(screenRatios);
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

void Window::create_window(std::string title) {
	this->title = title;
	this->screenDimensions = {WORLD_WIDTH, WORLD_HEIGHT};

	try {
		sdl_init();
		window_init();
	}
	catch (std::exception& e) {
		std::cerr << e.what() << "\n";
		exit(EXIT_FAILURE);
	}
	
	SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
	SDL_GetWindowSize(window, &screenDimensions.x, &screenDimensions.y);
	
	this->screenRatios.x = (double) screenDimensions.x / WORLD_WIDTH;
	this->screenRatios.y = (double) screenDimensions.y / WORLD_HEIGHT;
	Label::set_screen_ratios(screenRatios);
	this->isRunning = true;
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

TTF_Font* Window::load_font(const char* filepath, int size) {
	int rescaledSize = size * screenRatios.x;
	TTF_Font* font = NULL;
	font = TTF_OpenFont(filepath, rescaledSize);
	
	if (font == NULL)
		fatal_log(fmt::format("ERROR: Failed to create texture at {}\n{}\n", filepath, SDL_GetError()));
	
	return font;
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

void Window::render_color(SDL_Rect rect, SDL_Color color) {
	SDL_Rect resized = world_to_screen(rect);
	SDL_SetRenderDrawColor(
		renderer, 
		color.r, 
		color.g, 
		color.b, 
		color.a
	);
	SDL_RenderFillRect(renderer, &resized);
}

void Window::render_label(Label label) {
	SDL_Surface* textSurface = TTF_RenderText_Blended(
		label.get_font(), 
		label.get_text().c_str(), 
		label.get_color()
	);
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, textSurface);

	SDL_Rect dst;
	dst.x = label.get_dimensions().x * screenRatios.x;
	dst.y = label.get_dimensions().y * screenRatios.y;
	dst.w = textSurface->w;
	dst.h = textSurface->h;

	SDL_RenderCopy(renderer, texture, NULL, &dst);
	SDL_FreeSurface(textSurface);
	SDL_DestroyTexture(texture);
}

void Window::render_button(Button& button) {
	if (!button.is_visible()) return;

	if (button.is_pressed())
		render_color(button.get_dimensions(), button.get_pressed_color());
	else 
		render_color(button.get_dimensions(), button.get_released_color());
	
	render_label(button.get_label());
}

SDL_Rect Window::world_to_screen(SDL_Rect& rect) {
	SDL_Rect converted;
	converted.x = rect.x * screenRatios.x;
	converted.y = rect.y * screenRatios.y;
	converted.w = rect.w * screenRatios.x;
	converted.h = rect.h * screenRatios.y;
	return converted;
}

void Window::update() {
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
	SDL_SetRenderDrawColor(renderer, DARK_GREY.r, DARK_GREY.b, DARK_GREY.g, SDL_ALPHA_OPAQUE);
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
