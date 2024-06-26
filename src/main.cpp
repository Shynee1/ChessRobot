#include <time.h> 
#include "window.hpp"
#include "AssetPool.hpp"
#include "utility.hpp"
#include "spritesheet.hpp"
#include "BoardUI.hpp"
#include "opponent.hpp"
#include "mouseinput.hpp"
#include "boardinput.hpp"
#include "openingbook.hpp"

int main(int argc, char* args[]){
	srand(time(0));

	Window::Instance()->create_window("Chess", 1280, 720);
	AssetPool::Instance()->load_all_textures("assets\\textures"); 
	AssetPool::Instance()->add_font("assets\\fonts\\arial.ttf", 24);

	chess::Board board(STARTING_FEN);
	OpeningBook book(&board);
	MotorController motorController(&board);
	BoardUI boardUI(&board);
	MouseInput input(&board, &boardUI, &book);
	Opponent opponent(&board, &boardUI, &book, &motorController, 8);
	//BoardInput input(&board, &boardUI);

	motorController.home_machine();

	Uint64 currentTick = SDL_GetPerformanceCounter();
	Uint64 lastTick = 0;
	double deltaTime = 0;

	std::shared_ptr<Window> window = Window::Instance();
	SDL_Event event;
	while (window->is_running()){
		window->clear();

		lastTick = currentTick;
		currentTick = SDL_GetPerformanceCounter();
		deltaTime = (double)((currentTick - lastTick)*1000 / (double)SDL_GetPerformanceFrequency() );

		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_QUIT: 
				window->close(); 
				break;
			case SDL_MOUSEBUTTONDOWN:
				window->mouse_press_callback(event.button);
				break;
			case SDL_MOUSEBUTTONUP:
				window->mouse_unpress_callback(event.button);
				break;
			}
		}

		window->update(deltaTime);
		motorController.update(deltaTime);
		input.update(deltaTime);
		opponent.update(deltaTime);

		//motorController.graphics();
		opponent.graphics();
		boardUI.graphics();
		
		window->display();
	}
	
	return 0;
}

