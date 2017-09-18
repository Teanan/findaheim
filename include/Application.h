#ifndef APPLICATION_H_INCLUDED
#define APPLICATION_H_INCLUDED

#include <memory>

#include "ScenesManager.h"

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

class Application
{

public:

	Application();
	~Application();

	template <typename SceneT>
	void createScene(std::string name, std::shared_ptr<SceneT> T) {
		m_scenes.createScene(name, T);
	}
	void setCurrentScene(std::string name);
		
	void init();
	void run();

private:

	ScenesManager m_scenes;
	std::shared_ptr<Scene> m_currentScene;

	std::unique_ptr<sf::RenderWindow> m_window;
};

#endif // APPLICATION_H_INCLUDED
