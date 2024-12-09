#include "utility.hpp"
#include "window.hpp"
#include "assetpool.hpp"
#include "gui.hpp"
#include "gamemanager.hpp"
#include "boardui.hpp"
#include "button.hpp"
#include "mouseinput.hpp"
#include "boardinput.hpp"
#include "motorcontroller.hpp"
#include "opponent.hpp"

void register_button_callbacks();

int main (int argc, char* args[]){
	srand(time(0));

	auto window = Window::Instance();
	window->create_window("Chess");

	auto assetPool = AssetPool::Instance();
	assetPool->load_all_textures("assets/textures"); 

	std::vector<int> fontSizes = {38, 21, 24, 15, 31, 9};
	assetPool->add_font("assets/fonts/arial.ttf", 22);
	assetPool->load_font_sizes("assets/fonts/panton.ttf", fontSizes);

	auto gui = GUI::Instance();
	gui->create_ui();

	auto gameManager = GameManager::Instance();
	gameManager->add_component<BoardUI>();
	gameManager->add_component<MouseInput>();
	gameManager->add_component<MotorController>();
	gameManager->add_component<Opponent>();
	gameManager->add_component<BoardInput>();

	register_button_callbacks();

	SDL_Event event;
	while (window->is_running()){
		window->clear();

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
			case SDL_KEYUP:
				exit(EXIT_SUCCESS);
				break;
			}
		}

		window->update();
		gui->update();
		gameManager->update_components();

		gui->graphics();
		gameManager->draw_components();
		window->display();
	}
	
	return 0;
}

void register_button_callbacks() {
	auto gui = GUI::Instance();
	gui->assign_button_callback("machine", [](){
		GameManager::Instance()->get_component<MotorController>()->connect();
	});

	gui->assign_button_callback("board", []() {
		GameManager::Instance()->get_component<BoardInput>()->connect();
	});

	gui->assign_button_callback("home", [](){
		GameManager::Instance()->get_component<MotorController>()->home_machine();
	});

	gui->assign_button_callback("unlock", [](){
		GameManager::Instance()->get_component<MotorController>()->unlock();
	});

	gui->assign_button_callback("softReset", [](){
		GameManager::Instance()->get_component<BoardInput>()->reset();
	});

	gui->assign_button_callback("easy", [](){
		GameManager::Instance()->get_component<Opponent>()->set_stockfish_level(0);
	});

	gui->assign_button_callback("medium", [](){
		GameManager::Instance()->get_component<Opponent>()->set_stockfish_level(1);
	});

	gui->assign_button_callback("hard", [](){
		GameManager::Instance()->get_component<Opponent>()->set_stockfish_level(2);
	});

	gui->assign_button_callback("start", [](){
		GameManager::Instance()->start_game(STARTING_FEN);
		GUI::Instance()->get_button("start").set_visible(false);
		GUI::Instance()->get_button("reset").set_visible(true);
	});

	gui->assign_button_callback("reset", [](){
		GameManager::Instance()->end_game();
		GUI::Instance()->get_button("start").set_visible(true);
		GUI::Instance()->get_button("reset").set_visible(false);
	});
}
