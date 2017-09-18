#ifndef LIGHTCOMPONENT_H_INCLUDED
#define LIGHTCOMPONENT_H_INCLUDED

#include <anax/Component.hpp>

#include "TexturesManager.h"

struct LightComponent : anax::Component
{
	bool castShadow = true;
	bool emitPointLight = false;
	bool emitDirectionLight = false;

	Texture texture;

	sf::Color color = sf::Color::White;

	// for direction light
	sf::Vector2f castDirection = { 0.0f, 1.0f };
	float sourceRadius = 5.0f;
	float sourceDistance = 100.0f;

	static LightComponent pointLight(const Texture& t, const sf::Color& c) {
		LightComponent l;
		l.castShadow = false; l.emitPointLight = true; l.emitDirectionLight = false;
		l.texture = t;
		l.color = c;
		return l;
	}

	static LightComponent directionLight(const Texture& texture, const sf::Color& color, const sf::Vector2f& castDirection = { 0.0f, 1.0f }, float sourceRadius = 5.0f, float sourceDistance = 100.0f) {
		LightComponent l;
		l.castShadow = false; l.emitPointLight = false; l.emitDirectionLight = true;
		l.texture = texture;
		l.color = color;
		l.castDirection = castDirection;
		l.sourceRadius = sourceRadius;
		l.sourceDistance = sourceDistance;
		return l;
	}
};

#endif // LIGHTCOMPONENT_H_INCLUDED