#pragma once
#include "utility.hpp"
#include "window.hpp"
#include <filesystem>
#include <boost/filesystem.hpp>
#include <map>

#if defined(_WIN64)
	#define FILEPATH_SEPARATOR "\\"
#elif defined (_WIN32)
	#define FILEPATH_SEPARATOR "\\"
#else
	#define FILEPATH_SEPARATOR "/"
#endif
	
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
	void load_font_sizes(std::string filepath, std::vector<int> sizes);
	std::string get_absolute_path(std::string relativePath);
};
