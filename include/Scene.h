#ifndef SCENE_H_INCLUDED
#define SCENE_H_INCLUDED

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

class Scene
{

public:

	/// Default constructor
	Scene() : m_isActive(false) { }
	
	/// Used to init the scene. Called only one time before the first scene update
	/// \note You must define your own init method
	virtual void init() = 0;

	/// Load the scene. Called each time the current scene is changed to this one
	/// \note You must define your own load method
	virtual void load(sf::View screen) = 0;

	/// Unload the scene. Called each time the current scene is changed from this one
	/// \note You must define your own unload method
	virtual void unload() = 0;

	/// Updates the game
	/// \param deltaTime The change in time
	/// \note You must define your own update method
	virtual void update(float deltaTime) = 0;

	/// Renders the game
	/// \note You must define your own render method
	virtual void render(sf::RenderTarget& renderTarget) = 0;

	/// Handles SFML events
	/// \note You must define your own handleEvents method
	virtual void handleEvents(sf::Event events) = 0;

	/// \return true if this Scene is active
	bool isActive() const { return m_isActive; }

	void setActive(bool active) { m_isActive = active; }

	/// Quits the game
	void quit() { m_isActive = false; }

private:

	bool m_isActive;
};

#endif // SCENE_H_INCLUDED