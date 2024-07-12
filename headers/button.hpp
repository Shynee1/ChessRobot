#pragma once
#include "utility.hpp"
#include "label.hpp"

class Button {
private:
    std::function<void()> callback;
    SDL_Rect dimensions;
    SDL_Color pressedColor;
    SDL_Color releasedColor;
    Label text;
    bool isVisible;
    bool isPressed;
    bool justPressed;
public:
    Button() = default;
    Button(SDL_Rect dimensions, SDL_Color pressedColor, SDL_Color releasedColor);
    Button(SDL_Rect dimensions, SDL_Color pressedColor, SDL_Color releasedColor, Label text);    
    void set_pressed_color(SDL_Color pressedColor);
    void set_released_color(SDL_Color releasedColor);
    void set_text(Label newText);
    void set_visible(bool visible);
    void set_pressed(bool pressed);
    void set_callback(std::function<void()> callback);
    SDL_Rect get_dimensions();
    SDL_Color get_pressed_color();
    SDL_Color get_released_color();
    Label& get_label();
    bool is_visible();
    bool is_pressed();
    bool was_just_pressed();
    std::function<void()> get_callback();
};