#ifndef TEXTURESMANAGER_H_INCLUDED
#define TEXTURESMANAGER_H_INCLUDED

#include <map>
#include <string>
#include <memory>

#include <SFML/Graphics.hpp>

class Texture : public sf::Texture
{
public:

	struct TextureTransform {
		sf::Vector2f scale;
		sf::Vector2f origin;
		float rotation;
		TextureTransform() : scale({ 1, 1 }), origin({ 0, 0 }), rotation(0) {};
	};

	TextureTransform transform;

	Texture() {};
	Texture(sf::Texture const& t, TextureTransform tr) : sf::Texture(t), transform(tr) {};

	sf::Sprite getSprite() const{
		return applyTransform(sf::Sprite(*this));
	}

	sf::Sprite& applyTransform(sf::Sprite& sprite) const{
		sprite.setScale(transform.scale);
		sprite.setOrigin(transform.origin);
		sprite.setRotation(transform.rotation);
		return sprite;
	}

	Texture& centerOrigin() {
		transform.origin = sf::Vector2f(getSize()) * 0.5f;
		return *this;
	}

	Texture& setScale(const sf::Vector2f& v) {
		transform.scale = v;
		return *this;
	}

	Texture& setOrigin(const sf::Vector2f& v) {
		transform.origin = v;
		return *this;
	}

	Texture& setRotation(float r) {
		transform.rotation = r;
		return *this;
	}
};

class TexturesManager
{

public:

	class AliasNotFound : public std::exception
	{
	public:
		AliasNotFound(std::string alias) : m_alias(alias) {};

	private:
		std::string m_alias;
	};

	class IdNotFound : public std::exception
	{
	public:
		IdNotFound(unsigned int id) : m_id(id) {};

	private:
		unsigned int m_id;
	};

	struct TextureInfo
	{
		unsigned int id;
		std::string path;
		Texture::TextureTransform transform;
	};

	TexturesManager();
	~TexturesManager();

	Texture& getTexture(std::string alias);
	Texture& getTexture(unsigned int id);

	TextureInfo const& getTextureInfo(std::string alias);
	TextureInfo const& getTextureInfo(unsigned int id) const;

	unsigned int getId(std::string alias);

private:

	std::string lookForResources(std::string alias) const;
	unsigned int getFreeId() const;

	void loadTexture(unsigned int id, std::string path);

	std::map<unsigned int, TextureInfo> m_textures;

	std::map<std::string, unsigned int> m_alias;
	std::map<unsigned int, Texture> m_cachedTextures;

	unsigned int m_highId;

};

#endif // TEXTURESMANAGER_H_INCLUDED