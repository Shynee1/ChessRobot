#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <filesystem>
#include <map>
#include "utility.hpp"
#include "window.hpp"

class AssetPool {
private:
	static std::shared_ptr<AssetPool> s_instance;
	std::map<std::string, SDL_Texture*> m_textures;
	std::map<std::string, TTF_Font*> m_fonts;
public:
	static std::shared_ptr<AssetPool> Instance();
	AssetPool();
	~AssetPool();
	SDL_Texture* get_texture(std::string filename);
	SDL_Texture* add_texture(std::string filepath);
	void load_all_textures(std::string filepath);
	TTF_Font* get_font(std::string filename);
	TTF_Font* add_font(std::string filepath, int size);
};