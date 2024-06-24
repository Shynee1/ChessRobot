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

SDL_Texture* AssetPool::add_texture(Window* render_window, std::string fullPath) {
	size_t cutoffIndex = fullPath.find_last_of("\\") + 1;
	std::string filename = fullPath.substr(cutoffIndex, filename.size() - cutoffIndex);

	if (m_textures[filename] == NULL)
		m_textures[filename] = render_window->load_texture(fullPath.c_str());

	return m_textures[filename];
}

void AssetPool::load_all_textures(Window* render_window, std::string assets_path) {
	if (!std::filesystem::exists(assets_path)){
		std::cout << "ERROR: Cannot find path '" << assets_path << "'\n";
		return;
	}
		
	for (auto& entry : std::filesystem::directory_iterator(assets_path)) {
		std::string extension = entry.path().extension().string();
		if (extension == ".png" || extension == ".jpg")
			add_texture(render_window, entry.path().string());
	}
}