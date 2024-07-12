#include "button.hpp"

Button::Button(SDL_Rect dimensions, SDL_Color pressedColor, SDL_Color releasedColor) {
    this->dimensions = dimensions;
    this->pressedColor = pressedColor;
    this->releasedColor = releasedColor;
    this->isVisible = true;
    this->isPressed = false;
}

Button::Button(SDL_Rect dimensions, SDL_Color pressedColor, SDL_Color releasedColor, Label text) {
    SDL_Point newLabelPos;
    newLabelPos.x = dimensions.x + ((dimensions.w - text.get_dimensions().w) / 2.0);
    newLabelPos.y = dimensions.y + ((dimensions.h - text.get_dimensions().h) / 2.0);
    text.set_position(newLabelPos);
    
    this->dimensions = dimensions;
    this->pressedColor = pressedColor;
    this->releasedColor = releasedColor;
    this->text = text;
    this->isVisible = true;
    this->isPressed = false;
}   

void Button::set_pressed_color(SDL_Color newColor) {
    this->pressedColor = newColor;
}

void Button::set_released_color(SDL_Color newColor) {
    this->releasedColor = newColor;
}

void Button::set_text(Label newText) {
    this->text = newText;
}

void Button::set_visible(bool visible) {
    this->isVisible = visible;
}

void Button::set_pressed(bool pressed){
    if (this->isPressed && !pressed)
        this->justPressed = true;
    else
        this->justPressed = false;
    
    this->isPressed = pressed;
}

void Button::set_callback(std::function<void()> callback) {
    this->callback = callback;
}

SDL_Rect Button::get_dimensions() {
    return dimensions;
}

SDL_Color Button::get_pressed_color() {
    return pressedColor;
}

SDL_Color Button::get_released_color() {
    return releasedColor;
}

Label& Button::get_label() {
    return text;
}

bool Button::is_visible() {
    return isVisible;
}

bool Button::is_pressed() {
    return this->isPressed;
}

bool Button::was_just_pressed() {
    return this->justPressed;
}

std::function<void()> Button::get_callback() {
    return this->callback;
}