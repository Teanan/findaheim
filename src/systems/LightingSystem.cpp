#include "systems/LightingSystem.h"

LightingSystem::LightingSystem(){
	m_unshadowShader.loadFromFile("resources/unshadowShader.frag", sf::Shader::Fragment);
	m_lightOverShapeShader.loadFromFile("resources/lightOverShapeShader.frag", sf::Shader::Fragment);
	m_normalsShader.loadFromFile("resources/normalsShader.frag", sf::Shader::Fragment);

	m_penumbraTexture.loadFromFile("resources/penumbraTexture.png");
	m_penumbraTexture.setSmooth(true);

	m_lights.create(sf::FloatRect({ 0,0 }, { 640, 480 }), sf::Vector2u(640, 480), m_penumbraTexture, m_unshadowShader, m_lightOverShapeShader, m_normalsShader);
	m_lights.normalsEnabled(true);
}

void LightingSystem::update(double deltaTime) {
	for (auto& entity : getEntities())
	{
		auto& transform = entity.getComponent<TransformComponent>().transform;

		auto transformable = m_lightTransformables.find(entity.getId());
		if (transformable == m_lightTransformables.end()) continue;

		transformable->second->setPosition(transform.getTransform().transformPoint({ 0, 0 }));
		transformable->second->setRotation(transform.getRotation());
	}
}

void LightingSystem::onEntityAdded(anax::Entity &e){
	auto& entity = e.getComponent<LightComponent>();
	auto id = e.getId();

	if (entity.castShadow && e.hasComponent<SpriteComponent>()) {
		auto& sprite = e.getComponent<SpriteComponent>().sprite;

		m_shadowsEntities[id] = m_lights.allocateShape();
		m_shadowsEntities[id]->_shape = shapeFromRect(sf::FloatRect(sprite.getTextureRect()));

		m_lights.addShape(m_shadowsEntities[id]);
		m_lightTransformables.insert({ id, &m_shadowsEntities[id]->_shape });
	}

	if (entity.emitDirectionLight) {
		auto emitter = std::make_shared<ltbl::LightDirectionEmission>();
		emitter->_emissionSprite = entity.texture.getSprite();
		emitter->_emissionSprite.setColor(entity.color);
		emitter->_castDirection = entity.castDirection;
		emitter->_sourceRadius = entity.sourceRadius;
		emitter->_sourceDistance = entity.sourceDistance;

		m_lights.addLight(emitter);

		m_directionEmitter.insert({ id, emitter });
		m_lightTransformables.insert({ id, &emitter->_emissionSprite });
	}

	if (entity.emitPointLight) {
		auto emitter = std::make_shared<ltbl::LightPointEmission>();
		emitter->_emissionSprite = entity.texture.getSprite();
		emitter->_emissionSprite.setColor(entity.color);

		m_lights.addLight(emitter);

		m_pointEmitter.insert({ id, emitter });
		m_lightTransformables.insert({ id, &emitter->_emissionSprite });
	}
}

void LightingSystem::onEntityRemoved(anax::Entity &e){
	auto id = e.getId();

	if (m_shadowsEntities.count(id)) {
		m_lights.removeShape(m_shadowsEntities[id]);
		m_shadowsEntities.erase(id);
	}
	
	if (m_pointEmitter.count(id)) {
		m_lights.removeLight(m_pointEmitter[id]);
		m_pointEmitter.erase(id);
	}

	if (m_directionEmitter.count(id)) {
		m_lights.removeLight(m_directionEmitter[id]);
		m_directionEmitter.erase(id);
	}

	m_lightTransformables.erase(id);
}

void LightingSystem::render(sf::RenderTarget & renderTarget){
	sf::RenderStates lightRenderStates;
	lightRenderStates.blendMode = sf::BlendMultiply;

	m_lights.render(renderTarget.getView(), m_unshadowShader, m_lightOverShapeShader, m_normalsShader);

	sf::Sprite lights(m_lights.getLightingTexture());
	renderTarget.draw(lights, lightRenderStates);
}
