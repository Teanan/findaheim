#pragma once

#include <chipmunkpp/shape.hpp>
#include <chipmunkpp/body.hpp>

#ifndef _MSC_VER
#pragma GCC visibility push(default)
#endif
namespace cp {
	/// Box shape
	class BoxShape : public Shape {
	public:
		BoxShape(std::shared_ptr<Body> body, Float width, Float height, Float radius = 0)
			: Shape(cpBoxShapeNew(body ? (*body) : (cpBody*)0, width, height, radius), body)
		{
		}
	};
}
#ifndef _MSC_VER
#pragma GCC visibility pop
#endif