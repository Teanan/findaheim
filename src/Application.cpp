#include "Application.h"
#include "Scene.h"
#include "TexturesManager.h"

#include <chrono>

Application::Application(){

}

Application::~Application(){

}

void Application::setCurrentScene(std::string name){
	auto oldScene = m_currentScene;
	
	m_currentScene = m_scenes.getScene(name);
	if (m_currentScene == nullptr) return;

	m_scenes.init(name);

	if (oldScene) {
		oldScene->setActive(false);
		oldScene->unload();
	}

	m_currentScene->load(m_window->getView());
	m_currentScene->setActive(true);
}

void Application::init() {
	// init render
	sf::VideoMode videomode(640, 480);
	m_window.reset(new sf::RenderWindow(videomode, "Title"));

	setCurrentScene("test");
}

void Application::run() {
	const float MAX_FRAME_TIME = 1 / 4.f;
	const float DELTA_TIME = 1 / 60.f;
	double currentTime = 0; // Holds the current time
	double accumulator = 0; // Used to accumlate time in the game loop

	sf::Event event;
	while (m_currentScene->isActive())
	{
		while (m_window->pollEvent(event))
		{
			m_currentScene->handleEvents(event);
		}

		double newTime = std::chrono::duration_cast<std::chrono::duration<double>>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
		double frameTime = newTime - currentTime;
		currentTime = newTime;

		// cap the loop delta time
		if (frameTime >= MAX_FRAME_TIME)
		{
			frameTime = MAX_FRAME_TIME;
		}

		accumulator += frameTime;


		// Update our game
		while (accumulator >= DELTA_TIME)
		{
			m_currentScene->update(DELTA_TIME); // update the game (with the constant delta time)
			accumulator -= DELTA_TIME; // decrease the accumulator
		}

		m_currentScene->render(*m_window);
		m_window->display();
	}
}