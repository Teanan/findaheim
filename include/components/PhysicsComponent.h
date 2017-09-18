#ifndef PHYSICCOMPONENT_H_INCLUDED
#define PHYSICCOMPONENT_H_INCLUDED

#include <anax/Component.hpp>

#include <chipmunk.hpp>
#include "boxshape.h"

#include <vector>
#include <memory>

struct PhysicsComponent : anax::Component
{
	PhysicsComponent() : mass(1), moment(1), isStatic(true)
	{}

	std::shared_ptr<cp::CircleShape> addCircleShape(double radius, cp::Vect offset) {
		return std::dynamic_pointer_cast<cp::CircleShape>(*shapes.insert(shapes.end(), std::make_shared<cp::CircleShape>(nullptr, radius, offset)));
	}

	std::shared_ptr<cp::SegmentShape> addSegmentShape(cp::Vect a, cp::Vect b, double radius) {
		return std::dynamic_pointer_cast<cp::SegmentShape>(*shapes.insert(shapes.end(), std::make_shared<cp::SegmentShape>(nullptr, a, b, radius)));
	}

	std::shared_ptr<cp::BoxShape> addBoxShape(double width, double height, double radius = 0) {
		return std::dynamic_pointer_cast<cp::BoxShape>(*shapes.insert(shapes.end(), std::make_shared<cp::BoxShape>(nullptr, width, height, radius)));
	}

	std::shared_ptr<cp::PolyShape> addPolyShape(const std::vector<cp::Vect>& verts) {
		return std::dynamic_pointer_cast<cp::PolyShape>(*shapes.insert(shapes.end(), std::make_shared<cp::PolyShape>(nullptr, verts)));
	}

	PhysicsComponent& createBody(double _mass, double _moment) {
		mass = _mass;
		moment = _moment;
		isStatic = false;

		return *this;
	}

	double mass;
	double moment;

	bool isStatic;

	std::vector<std::shared_ptr<cp::Shape>> shapes;
};

#endif // PHYSICCOMPONENT_H_INCLUDED
