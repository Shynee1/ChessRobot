#include "gui.hpp"

std::shared_ptr<GUI> GUI::instance = NULL;

std::shared_ptr<GUI> GUI::Instance(){
	if (instance == NULL)
		instance = std::make_shared<GUI>();

	return instance;
}

void GUI::create_ui() {
    auto assetPool = AssetPool::Instance();

	SDL_Texture* boardTex = assetPool->get_texture("Board.png");
	this->boardTexture = std::make_unique<RenderObject>(boardTex);
	this->boardTexture->set_dimensions(WORLD_HEIGHT, WORLD_HEIGHT);
	this->boardTexture->align_center();

    TTF_Font* arial    = assetPool->get_font("arial22");
	TTF_Font* panton38 = assetPool->get_font("panton38");
	TTF_Font* panton31 = assetPool->get_font("panton31");
	TTF_Font* panton24 = assetPool->get_font("panton24");
	TTF_Font* panton21 = assetPool->get_font("panton21");
	TTF_Font* panton15 = assetPool->get_font("panton15");
	TTF_Font* panton9  = assetPool->get_font("panton9");

	add_label("heading", {{1015, 15}, "Chess Robot", panton38, LIGHT_BLUE});
	add_label("subheading", {{1014, 56}, "The Haverford School", panton21, LIGHT_GREY});
	add_label("line1", {{1000, 93}, "__________________________________________________________________", panton9, LIGHT_GREY});
	add_label("machine", {{1014, 129}, "Machine: ", panton24, LIGHT_GREY});
	add_label("board", {{1014, 171}, "Board: ", panton24, LIGHT_GREY});
	add_label("line2", {{1000, 215}, "__________________________________________________________________", panton9, LIGHT_GREY});
	add_label("machineCommands", {{1018, 248}, "Machine Commands", panton24, LIGHT_GREY});
	add_label("engineSettings", {{1028, 370}, "Engine Difficulty", panton24, LIGHT_GREY});

	Label connect({0, 0}, "Connect", panton15, DARK_GREY);
	Label home({0, 0}, "Home", panton15, DARK_GREY);
	Label unlock({0, 0}, "Unlock", panton15, DARK_GREY);
	Label softReset({0, 0}, "Reset", panton15, DARK_GREY);
	Label easy({0, 0}, "Easy", panton15, DARK_GREY);
	Label medium({0, 0}, "Medium", panton15, DARK_GREY);
	Label hard({0, 0}, "Hard", panton15, DARK_GREY);
	Label start({0, 0}, "start", panton31, WHITE);
	Label reset({0, 0}, "reset", panton31, WHITE);

	add_button("machine", {{1141, 130, 118, 26}, DARK_GREY, WHITE, connect});
	add_button("board", {{1141, 173, 118, 26}, DARK_GREY, WHITE, connect});
	add_button("home", {{1015, 286, 75, 45}, DARK_GREY, WHITE, home});
	add_button("unlock", {{1103, 286, 75, 45}, DARK_GREY, WHITE, unlock});
	add_button("softReset", {{1191, 286, 75, 45}, DARK_GREY, WHITE, softReset});
	add_button("easy", {{1015, 406, 75, 45}, DARK_GREY, WHITE, easy});
	add_button("medium", {{1103, 406, 75, 45}, DARK_GREY, WHITE, medium});
	add_button("hard", {{1191, 406, 75, 45}, DARK_GREY, WHITE, hard});
	add_button("start", {{1020, 659, 250, 45}, DARK_GREY, LIGHT_BLUE, start});
	add_button("reset", {{1020, 659, 250, 45}, DARK_GREY, LIGHT_BLUE, reset});
	get_button("reset").set_visible(false);

	add_label("machineConnectedText", {{TEXT_PADDING_X, 20}, "Machine:", arial, LIGHT_GREY});
	add_label("machineConnected", {{119, 22}, "Disconnected", arial, LIGHT_GREY});
	add_label("boardConnectedText", {{TEXT_PADDING_X, 61}, "Board:", arial, LIGHT_GREY});
	add_label("boardConnected", {{119, 61}, "Disconnected", arial, LIGHT_GREY});
	add_label("line3", {{0, 94}, "________________________________________________________", panton9, LIGHT_GREY});
	add_label("skillLevel", {{TEXT_PADDING_X, 123}, "Skill Level: 0", arial, LIGHT_GREY});
	add_label("targetDepth", {{TEXT_PADDING_X, 162}, "Target Depth: 0", arial, LIGHT_GREY});
	add_label("thinkTime", {{TEXT_PADDING_X, 206}, "Think Time: 0", arial, LIGHT_GREY});
	add_label("line4", {{0, 239}, "________________________________________________________", panton9, LIGHT_GREY});
	add_label("bestMove", {{TEXT_PADDING_X, 267}, "Best Move: N/A", arial, LIGHT_GREY});
	add_label("depth", {{TEXT_PADDING_X, 306}, "Depth: Book", arial, LIGHT_GREY});
	add_label("score", {{TEXT_PADDING_X, 350}, "Evaluation: Book", arial, LIGHT_GREY});
	add_label("line5", {{0, 383}, "________________________________________________________", panton9, LIGHT_GREY});
	add_label("x", {{TEXT_PADDING_X, 412}, "X: 0.00", arial, LIGHT_GREY});
	add_label("y", {{TEXT_PADDING_X, 451}, "Y: 0.00", arial, LIGHT_GREY});
	add_label("z", {{TEXT_PADDING_X, 490}, "Z: 0.00", arial, LIGHT_GREY});
}

void GUI::update() {
   for (auto& entry : buttons) {
		Button& button = entry.second;

		if (!button.is_visible()) continue;

		auto window = Window::Instance();

		SDL_Point mousePos = window->get_mouse_pos();
		SDL_Rect dimensions = button.get_dimensions();
		if (!SDL_PointInRect(&mousePos, &dimensions))
			continue;
		
		if (window->is_mouse_pressed())
			button.set_pressed(true);
		else 
			button.set_pressed(false);

		if (button.was_just_pressed())
			button.get_callback()();
   }
}

void GUI::graphics() {
	Window::Instance()->render_object(boardTexture.get());

    for (auto& entry : buttons) 
		Window::Instance()->render_button(entry.second);

	for (auto& entry :labels) 
		Window::Instance()->render_label(entry.second);
}

void GUI::add_button(std::string id, Button button) {
	if (buttons.count(id) != 0)
        fatal_log(fmt::format("ERROR: Button with id '{}' already exists", id));

	buttons[id] = button;
}

void GUI::add_label(std::string id, Label label) {
	if (labels.count(id) != 0)
        fatal_log(fmt::format("ERROR: Label with id '{}' already exists", id));

	labels[id] = label;
}

Button& GUI::get_button(std::string id) {
    if (buttons.count(id) == 0)
        fatal_log(fmt::format("ERROR: No button with id '{}' found", id));

	return buttons[id];
}

Label& GUI::get_label(std::string id) {
    if (labels.count(id) == 0)
        fatal_log(fmt::format("ERROR: No label with id '{}' found", id));

	return labels[id];
}

void GUI::assign_button_callback(std::string id, std::function<void()> callback){
	get_button(id).set_callback(callback);
}
