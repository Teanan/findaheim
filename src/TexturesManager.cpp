#include "TexturesManager.h"

#include <fstream>

TexturesManager::TexturesManager()
	: m_highId(0)
{

}

TexturesManager::~TexturesManager(){

}

Texture& TexturesManager::getTexture(std::string alias){
	return getTexture(getId(alias));
}

Texture& TexturesManager::getTexture(unsigned int id){
	if (m_cachedTextures.count(id))
		return m_cachedTextures.at(id);

	m_cachedTextures[id] = Texture();
	m_cachedTextures[id].transform = getTextureInfo(id).transform;
	m_cachedTextures[id].setSmooth(true);
	
	if (!m_cachedTextures[id].loadFromFile(getTextureInfo(id).path)) {
		m_cachedTextures.erase(id);

		throw std::exception(std::string("failed to load " + getTextureInfo(id).path).c_str());
	}

	return m_cachedTextures[id];
}

TexturesManager::TextureInfo const & TexturesManager::getTextureInfo(std::string alias){
	return getTextureInfo(getId(alias));
}

TexturesManager::TextureInfo const & TexturesManager::getTextureInfo(unsigned int id) const{
	if (!m_textures.count(id))
		throw IdNotFound(id);

	return m_textures.at(id);
}

unsigned int TexturesManager::getId(std::string alias){
	if (!m_alias.count(alias)) {
		auto res = lookForResources(alias);
		
		if (res == "")
			throw AliasNotFound(alias);

		m_alias[alias] = getFreeId();
		if (m_alias[alias] > m_highId) m_highId = m_alias[alias];

		loadTexture(m_alias[alias], res);
	}

	return m_alias.at(alias);
}

std::string TexturesManager::lookForResources(std::string alias) const {
	for (auto ext : { "", ".png", ".jpg", ".jpeg", ".gif" }) {
		std::ifstream stream(alias + ext);

		if (stream.good())
			return alias + ext;
	}

	return "";
}

unsigned int TexturesManager::getFreeId() const{
	if (m_highId == m_textures.size())
		return m_highId + 1;

	unsigned int id = 0;

	while (true) {
		id++;

		if (m_textures.count(id) == 0) return id;
	}

	return 0;
}

void TexturesManager::loadTexture(unsigned int id, std::string path){
	m_textures[id] = { id, path };
}