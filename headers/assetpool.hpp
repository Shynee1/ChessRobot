#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <filesystem>
#include <map>
#include "utility.hpp"
#include "window.hpp"

class AssetPool {
private:
	static std::shared_ptr<AssetPool> s_instance;
	std::map<std::string, SDL_Texture*> m_textures;
public:
	static std::shared_ptr<AssetPool> Instance();
	AssetPool();
	~AssetPool();
	SDL_Texture* get_texture(std::string filename);
	SDL_Texture* add_texture(Window* render_window, std::string filename);
	void load_all_textures(Window* render_window, std::string assets_path);
};