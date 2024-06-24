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

	Window window("Chess", 1280, 720);
	AssetPool::Instance()->load_all_textures(&window, "assets\\textures"); 

	chess::Board board(STARTING_FEN);
	OpeningBook book(&board);
	MotorController motorController(&board);
	BoardUI boardUI(&board, &window);
	MouseInput input(&board, &window, &boardUI, &book);
	Opponent opponent(&board, &boardUI, &book, &motorController, 8);
	//BoardInput input(&board, &boardUI);

	uint32_t prevTime = SDL_GetTicks();
	float deltaTime = 0;
	SDL_Event event;

	//motorController.home_machine();

	while (window.is_running()){
		window.clear();

		uint32_t newTime = SDL_GetTicks();
		deltaTime = (newTime - prevTime) / 1000.0f;
		prevTime = newTime;

		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_QUIT: 
				window.close(); 
				break;
			case SDL_MOUSEBUTTONDOWN:
				window.mouse_press_callback(event.button);
				break;
			case SDL_MOUSEBUTTONUP:
				window.mouse_unpress_callback(event.button);
				break;
			}
		}

		window.update(deltaTime);
		//motorController.update(deltaTime);
		input.update(deltaTime);
		opponent.update(deltaTime);
		boardUI.graphics();
		
		window.display();
	}
	
	return 0;
}

