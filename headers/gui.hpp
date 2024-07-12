#pragma once
#include "label.hpp"
#include "button.hpp"
#include "assetpool.hpp"

constexpr int TEXT_PADDING_X = 10;

class GUI {
private:
    static std::shared_ptr<GUI> instance;

    std::unique_ptr<RenderObject> boardTexture;
    std::map<std::string, Button> buttons = {};
	std::map<std::string, Label>  labels = {};
public:
    static std::shared_ptr<GUI> Instance();
    GUI() = default;
    void create_ui();
    void update();
    void graphics();
    void add_button(std::string id, Button button);
	void add_label(std::string id, Label label);
    Button& get_button(std::string id);
	Label& get_label(std::string id);
    void assign_button_callback(std::string id, std::function<void()> callback);
};