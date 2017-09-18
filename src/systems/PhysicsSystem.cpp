#include <Systems/PhysicsSystem.h>

#include <chipmunk_types.h>

#include "MathTools.h"

#include <iostream>
#include <iomanip>

PhysicsSystem::PhysicsSystem(){	
	cpSpaceSetIterations(m_space, 30);
	cpSpaceSetSleepTimeThreshold(m_space, 0.5f);
	cpSpaceSetCollisionSlop(m_space, 0.5f);
}

void PhysicsSystem::update(double deltaTime){
	m_space.step(deltaTime);

	auto& entities = getEntities();
	for (auto& entity : entities){
		auto body = m_bodies.find(entity.getId());
		if (body == m_bodies.end()) continue;
		if (!body->second) continue;

		auto& transform = entity.getComponent<TransformComponent>().transform;

		transform.setPosition(body->second->getPosition().x, body->second->getPosition().y);
		transform.setRotation(cpBodyGetAngle(*body->second) * _radToDeg);
	}
}

void PhysicsSystem::onEntityAdded(anax::Entity& e){
	PhysicsComponent& physic = e.getComponent<PhysicsComponent>();
	auto& transform = e.getComponent<TransformComponent>().transform;

	if (!physic.isStatic) {
		auto body = m_bodies.insert({ e.getId(), std::make_shared<cp::Body>(physic.mass, physic.moment) }).first->second;

		m_space.add(body);

		body->setPosition({ transform.getPosition().x, transform.getPosition().y });

		for (auto& shape : physic.shapes)
			shape->setBody(body);
	}
	else
		for (auto& shape : physic.shapes) {
		//	std::shared_ptr<cp::Shape>& shape = *it;

			auto shapeTranslate = cpTransformTranslate(cp::Vect(transform.getPosition().x + transform.getOrigin().x, transform.getPosition().y + transform.getOrigin().y));
			auto shapeRotation = cpTransformRotate(transform.getRotation() * _radToDeg);
			auto shapeTransform = cpTransformWrap(shapeRotation, shapeTranslate);
			cpShapeUpdate(*shape, shapeTransform);

			auto a = cpSegmentShapeGetA(*shape);
			auto b = cpSegmentShapeGetB(*shape);

			a = cpTransformPoint(shapeTransform, a);
			b = cpTransformPoint(shapeTransform, b);

			shape = std::dynamic_pointer_cast<cp::Shape>(std::make_shared<cp::SegmentShape>(m_space.staticBody, a, b, cpSegmentShapeGetRadius(*shape)));

			shape->setBody(m_space.staticBody);

			//if (static_cast<cpSegmentShape*>(*shape))
		}

	for (auto& shape : physic.shapes)
		m_space.add(shape);
}

void PhysicsSystem::onEntityRemoved(anax::Entity& e){
	auto body = m_bodies.find(e.getId());
	if (body == m_bodies.end()) return;
	if (!body->second) return;

	m_space.remove(body->second);
	m_bodies.erase(body);
}

void PhysicsSystem::setGravity(const cp::Vect& g) {
	m_space.setGravity(g);
}