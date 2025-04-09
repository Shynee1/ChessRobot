#include "label.hpp"

SDL_FPoint Label::screenRatios(1, 1);

Label::Label(SDL_Point pos, std::string text, TTF_Font* font) {
    this->dimensions.x = pos.x;
    this->dimensions.y = pos.y;
    TTF_SizeText(font, text.c_str(), &this->dimensions.w, &this->dimensions.h);
    
    this->dimensions.w *= (1 / Label::screenRatios.x);
    this->dimensions.h *= (1 / Label::screenRatios.y);

    this->text = text;
    this->font = font;
    this->color = BLACK;
}

Label::Label(SDL_Point pos, std::string text, TTF_Font* font, SDL_Color color) {
    this->dimensions.x = pos.x;
    this->dimensions.y = pos.y;
    TTF_SizeText(font, text.c_str(), &this->dimensions.w, &this->dimensions.h);
    
    this->dimensions.w *= (1 / Label::screenRatios.x);
    this->dimensions.h *= (1 / Label::screenRatios.y);

    this->text = text;
    this->font = font;
    this->color = BLACK;
    this->color = color;
}

void Label::set_text(std::string newText) {
    this->text = newText;
    TTF_SizeText(font, text.c_str(), &this->dimensions.w, &this->dimensions.h);
    this->dimensions.w *= (1 / Label::screenRatios.x);
    this->dimensions.h *= (1 / Label::screenRatios.y);
}

void Label::set_position(SDL_Point newPos) {
    this->dimensions.x = newPos.x;
    this->dimensions.y = newPos.y;
}

void Label::set_font(TTF_Font* newFont) {
    this->font = newFont;
}

void Label::set_color(SDL_Color newColor) {
    this->color = newColor;
}

void Label::center_horizontally() {
    this->dimensions.x = (int) ((WORLD_WIDTH / 2.0) - (dimensions.w / 2.0));
}

std::string Label::get_text() {
    return this->text;
}

SDL_Rect Label::get_dimensions() {
    return this->dimensions;
}

TTF_Font* Label::get_font() {
    return this->font;
}

SDL_Color Label::get_color() {
    return this->color;
}
