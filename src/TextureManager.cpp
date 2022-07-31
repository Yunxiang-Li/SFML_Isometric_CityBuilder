#include "TextureManager.hpp"

std::shared_ptr<TextureManager> TextureManager::m_instance = nullptr;

std::shared_ptr<TextureManager> TextureManager::getInstance()
{
	// For the first call, create a new TextureManager object and set it inside the std::shared_ptr.
	if (!m_instance)
		m_instance.reset(new TextureManager);
	// For subsequent calls, return the already created one.
	return m_instance;
}

void TextureManager::loadTexture(const std::string& texture_name, const std::string& file_name)
{
	// Load the input texture from the input file.
	sf::Texture texture;
	texture.loadFromFile(file_name);
	// Store (texture name, texture) as a key-value pair inside the m_str_texture_map.
	getInstance()->m_str_texture_map[texture_name] = texture;
}

sf::Texture& TextureManager::getTextureRef(const std::string& texture_str)
{
	return getInstance()->m_str_texture_map.at(texture_str);
}