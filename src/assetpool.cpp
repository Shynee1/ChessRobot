#include "assetpool.hpp"

std::shared_ptr<AssetPool> AssetPool::s_instance = NULL;

std::shared_ptr<AssetPool> AssetPool::Instance() {
	if (s_instance == NULL) 
		s_instance = std::make_shared<AssetPool>();
	
	return s_instance;
}

AssetPool::AssetPool() {
	
}

AssetPool::~AssetPool() {
	for (auto& texture : m_textures) {
		if (texture.second != NULL) 
			SDL_DestroyTexture(texture.second);
	}

	for (auto& font : m_fonts){
		if (font.second != NULL)
			TTF_CloseFont(font.second);
	}

	m_textures.clear();
	m_fonts.clear();
}

SDL_Texture* AssetPool::get_texture(std::string filename) {
	if (m_textures[filename] == NULL) 
		fatal_log(fmt::format("ERROR: No texture with name '{}'", filename));
	return m_textures[filename];
}

SDL_Texture* AssetPool::add_texture(std::string filepath) {
	filepath = get_absolute_path(filepath);
	
	if (!std::filesystem::exists(filepath))
		fatal_log(fmt::format("ERROR: Cannot find path '{}'", filepath));

	size_t cutoffIndex = filepath.find_last_of(FILEPATH_SEPARATOR) + 1;
	std::string filename = filepath.substr(cutoffIndex);

	if (m_textures[filename] == NULL)
		m_textures[filename] = Window::Instance()->load_texture(filepath.c_str());

	return m_textures[filename];
}

void AssetPool::load_all_textures(std::string filepath) {
	filepath = get_absolute_path(filepath);
	
	for (auto& entry : std::filesystem::directory_iterator(filepath)) {
		std::string extension = entry.path().extension().string();
		if (extension == ".png" || extension == ".jpg")
			add_texture(entry.path().string());
	}
}

TTF_Font* AssetPool::get_font(std::string filename) {
	if (m_fonts[filename] == NULL) 
		fatal_log(fmt::format("ERROR: No font with name '{}'", filename));
	return m_fonts[filename];
}

TTF_Font* AssetPool::add_font(std::string filepath, int size) {
	filepath = get_absolute_path(filepath);
	
	if (!std::filesystem::exists(filepath))
		fatal_log(fmt::format("ERROR: Cannot find path '{}'", filepath));

	size_t startIndex = filepath.find_last_of(FILEPATH_SEPARATOR) + 1;
	size_t endIndex = filepath.find_last_of(".");
	std::string sizeString = std::to_string(size);
	std::string filename = filepath.substr(startIndex, endIndex - startIndex) + sizeString;

	if (m_fonts[filename] == NULL)
		m_fonts[filename] = Window::Instance()->load_font(filepath.c_str(), size);
	
	return m_fonts[filename];
}

void AssetPool::load_font_sizes(std::string filepath, std::vector<int> sizes) {
	for (int& size : sizes)
		add_font(filepath, size);
}

std::string AssetPool::get_absolute_path(std::string relativePath){
	std::string systemSpecific = replaceAll(relativePath, "/", FILEPATH_SEPARATOR);
	systemSpecific = replaceAll(systemSpecific, "\\", FILEPATH_SEPARATOR);
	
	return boost::filesystem::system_complete(systemSpecific).string();
}
	
	
