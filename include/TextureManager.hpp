#pragma once
#ifndef TEXTUREMANAGER_HPP
#define TEXTUREMANAGER_HPP

#include <memory>
#include <string>
#include <SFML/Graphics.hpp>
#include <unordered_map>

/**
 * A class manages all textures' load, store and retrieve behaviors by singleton pattern.
 */
class TextureManager
{
 public:

	/**
	  * The static method that controls the access to the TextureManager's singleton instance.
	 * On the first run, it creates a singleton object and places it into the static field.
	 * On subsequent runs, it returns the only TextureManager object stored in the static field.
	 * @return A std::shared_ptr<TextureManager> indicates the TextureManager instance.
	 */
	static std::shared_ptr<TextureManager> getInstance();

	/**
	 * Deleted copy constructor since a singleton cannot be copyable.
	 */
	TextureManager(const TextureManager&) = delete;
	/**
	 * Deleted copy assignment operator since a singleton cannot be assignable.
	 */
	void operator=(const TextureManager&) = delete;

	/**
	 * Load a texture from a file .
	 * @param texture_name A const reference of std::string indicates the loaded sf::Texture object's name.
	 * @param file_name A const reference of std::string indicates the file name.
	 */
	static void loadTexture(const std::string& texture_name, const std::string& file_name);

	/**
	 * Retrieve and return the reference of sf::Texture object related to input string.
	 * @param texture_str A const reference of std::string indicates the name of required sf::Texture object.
	 * @return A reference of required sf::Texture object.
	 */
	static sf::Texture& getTextureRef(const std::string& texture_str) ;

 private:
	/**
 	 * Hide the constructor of TextureManager class.
 	 */
	TextureManager() = default;

	// A std::shared_ptr<TextureManager> object indicates the singleton instance of TextureManager.
	static std::shared_ptr<TextureManager> m_instance;

	// An unordered_map that uses each texture's string name as key and related texture object as value.
	std::unordered_map<std::string, sf::Texture> m_str_texture_map;
};

#endif //TEXTUREMANAGER_HPP
