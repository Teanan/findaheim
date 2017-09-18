#ifndef PHYSICSSYSTEM_H_INCLUDED
#define PHYSICSSYSTEM_H_INCLUDED

#include <anax/System.hpp>

#include <Components/TransformComponent.hpp>
#include <Components/PhysicsComponent.h>

#include <chipmunk.hpp>

#include <map>
#include <memory>

struct PhysicsSystem : anax::System<anax::Requires<TransformComponent, PhysicsComponent>>
{
public:

	PhysicsSystem();

	void update(double deltaTime);
	
	void onEntityAdded(anax::Entity&);
	void onEntityRemoved(anax::Entity&);

	void setGravity(const cp::Vect&);

private:

	cp::Space m_space;
	std::shared_ptr<cp::Body> ballBody;

	std::map<anax::Entity::Id, std::shared_ptr<cp::Body>> m_bodies;
};

#endif // PHYSICSSYSTEM_H_INCLUDED
