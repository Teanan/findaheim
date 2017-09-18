#include "ScenesManager.h"

ScenesManager::ScenesManager(){

}

ScenesManager::~ScenesManager(){

}

void ScenesManager::deleteScene(std::string name){
	if (!m_scenes.count(name))
		throw std::exception(std::string("Scene " + name + " not found").c_str());

	m_scenes.erase(name);
}

void ScenesManager::init(std::string name){
	if (m_initiatedScenes.count(name))
		return;

	if (getScene(name) == nullptr)
		return;

	getScene(name)->init();
	
	m_initiatedScenes[name] = true;
}
