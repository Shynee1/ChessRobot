#include "AssetPool.hpp"

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

	m_textures.clear();
}

SDL_Texture* AssetPool::get_texture(std::string filename) {
	if (m_textures[filename] == NULL) 
		std::cerr << "No texture with name " << filename << "\n";
	return m_textures[filename];
}

SDL_Texture* AssetPool::add_texture(std::string filepath) {
	size_t cutoffIndex = filepath.find_last_of("\\") + 1;
	std::string filename = filepath.substr(cutoffIndex, filename.size() - cutoffIndex);

	if (m_textures[filename] == NULL)
		m_textures[filename] = Window::Instance()->load_texture(filepath.c_str());

	return m_textures[filename];
}

void AssetPool::load_all_textures(std::string filepath) {
	if (!std::filesystem::exists(filepath)){
		std::cout << "ERROR: Cannot find path '" << filepath << "'\n";
		return;
	}
		
	for (auto& entry : std::filesystem::directory_iterator(filepath)) {
		std::string extension = entry.path().extension().string();
		if (extension == ".png" || extension == ".jpg")
			add_texture(entry.path().string());
	}
}

TTF_Font* AssetPool::get_font(std::string filename) {
	if (m_fonts[filename] == NULL) 
		std::cerr << "No font with name " << filename << "\n";
	return m_fonts[filename];
}

TTF_Font* AssetPool::add_font(std::string filepath, int size) {
	size_t startIndex = filepath.find_last_of("\\") + 1;
	size_t endIndex = filepath.find_last_of(".");
	std::string sizeString = std::to_string(size);
	std::string filename = filepath.substr(startIndex, endIndex - startIndex) + sizeString;

	if (m_fonts[filename] == NULL)
		m_fonts[filename] = TTF_OpenFont(filepath.c_str(), size);

	return m_fonts[filename];
}