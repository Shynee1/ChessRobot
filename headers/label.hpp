#pragma once
#include "utility.hpp"



class Label {
private:
    static SDL_FPoint screenRatios;
    SDL_Rect dimensions;
    std::string text;
    TTF_Font* font;
    SDL_Color color;
public:
    Label() = default;
    Label(SDL_Point pos, std::string text, TTF_Font* font);
    Label(SDL_Point pos, std::string text, TTF_Font* font, SDL_Color color);
    void set_text(std::string newText);
    void set_position(SDL_Point newPos);
    void set_font(TTF_Font* newFont);
    void set_color(SDL_Color newColor);
    std::string get_text();
    SDL_Rect get_dimensions();
    TTF_Font* get_font();
    SDL_Color get_color();
    static void set_screen_ratios(SDL_FPoint screenRatios) {
        Label::screenRatios = screenRatios;
    }
};
