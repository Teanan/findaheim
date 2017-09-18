#ifndef SCENESMANAGER_H_INCLUDED
#define SCENESMANAGER_H_INCLUDED

#include <map>
#include <string>
#include <memory>

#include "Scene.h"

class ScenesManager
{

public:

	ScenesManager();
	~ScenesManager();

	template <typename SceneType>
	std::shared_ptr<Scene> createScene(std::string name, std::shared_ptr<SceneType> newScene) {
		if (m_scenes.count(name))
			throw std::exception(std::string("Scene " + name + " already exists").c_str());

		m_scenes[name] = std::dynamic_pointer_cast<Scene>(newScene);

		return m_scenes[name];
	}

	template <typename SceneType>
	std::shared_ptr<SceneType> getScene(std::string name) {
		return std::dynamic_pointer_cast<SceneType>(getScene(name));
	}

	std::shared_ptr<Scene> getScene(std::string name) {
		if (!m_scenes.count(name))
			throw std::exception(std::string("Scene " + name + " not found").c_str());

		return m_scenes.at(name);
	}

	void deleteScene(std::string name);

	void init(std::string name);

private:

	std::map<std::string, std::shared_ptr<Scene>> m_scenes;

	std::map<std::string, bool> m_initiatedScenes;
};

#endif // SCENESMANAGER_H_INCLUDED