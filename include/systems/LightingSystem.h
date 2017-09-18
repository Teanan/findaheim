///
/// anax
/// An open source C++ entity system.
///
/// Copyright (C) 2013-2014 Miguel Martin (miguel@miguel-martin.com)
///
/// Permission is hereby granted, free of charge, to any person obtaining a copy
/// of this software and associated documentation files (the "Software"), to deal
/// in the Software without restriction, including without limitation the rights
/// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
/// copies of the Software, and to permit persons to whom the Software is
/// furnished to do so, subject to the following conditions:
///
/// The above copyright notice and this permission notice shall be included in
/// all copies or substantial portions of the Software.
///
/// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
/// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
/// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
/// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
/// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
/// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
/// THE SOFTWARE.
///

#ifndef LIGHTINGSYSTEM_H_INCLUDED
#define LIGHTINGSYSTEM_H_INCLUDED

#include <anax/System.hpp>

#include <SFML/Graphics.hpp>

#include "lighting/LightEngine.h"

#include "Components/LightComponent.h"
#include "Components/SpriteComponent.hpp"
#include "Components/TransformComponent.hpp"

struct LightingSystem : anax::System<anax::Requires<LightComponent, TransformComponent>>
{
	/// Construct a SpriteRenderingSystem with no render target
	LightingSystem();

	/// Renders the system
	void render(sf::RenderTarget& renderTarget);

	void update(double deltaTime);

	void onEntityAdded(anax::Entity&);
	void onEntityRemoved(anax::Entity&);
	
private:

	sf::Shader m_unshadowShader;
	sf::Shader m_lightOverShapeShader;
	sf::Shader m_normalsShader;
	sf::Texture m_penumbraTexture;

	LightEngine m_lights;

	std::map<anax::Entity::Id, LightShape*> m_shadowsEntities;
	std::map<anax::Entity::Id, std::shared_ptr<LightPointEmission>> m_pointEmitter;
	std::map<anax::Entity::Id, std::shared_ptr<LightDirectionEmission>> m_directionEmitter;

	std::map<anax::Entity::Id, sf::Transformable*> m_lightTransformables;
};

#endif // LIGHTINGSYSTEM_H_INCLUDED
