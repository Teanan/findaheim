#pragma once

#include <SFML/Graphics.hpp>

#include <assert.h>

constexpr const float _pi = 3.14159265f;
constexpr const float _radToDeg = 180.0f / _pi;

sf::ConvexShape shapeFixWinding(const sf::ConvexShape &shape);
bool rayIntersect(const sf::Vector2f &as, const sf::Vector2f &ad, const sf::Vector2f &bs, const sf::Vector2f &bd, sf::Vector2f &intersection);

template <typename T, typename I>
constexpr inline sf::Vector2<T> rectHalfDims(const sf::Rect<I> &rect) {
	return { rect.width * 0.5f, rect.height * 0.5f };
}

template <typename T>
constexpr inline sf::Vector2<T> rectHalfDims(const sf::Rect<T> &rect) {
	return sf::Vector2<T>(rect.width * 0.5f, rect.height * 0.5f);
}

template <typename T>
constexpr inline sf::Vector2<T> rectCenter(const sf::Rect<T> &rect) {
	return sf::Vector2<T>(rect.left + rect.width * 0.5f, rect.top + rect.height * 0.5f);
}

template <typename T>
inline bool rectIntersects(const sf::Rect<T> &rect, const sf::Rect<T> &other) {
	if (rect.left + rect.width < other.left)
		return false;
	if (rect.top + rect.height < other.top)
		return false;
	if (rect.left > other.left + other.width)
		return false;
	if (rect.top > other.top + other.height)
		return false;

	return true;
}

template <typename T>
inline bool rectContains(const sf::Rect<T> &rect, const sf::Rect<T> &other) {
	if (other.left < rect.left)
		return false;
	if (other.top < rect.top)
		return false;
	if (other.left + other.width > rect.left + rect.width)
		return false;
	if (other.top + other.height > rect.top + rect.height)
		return false;

	return true;
}

template <typename T>
constexpr inline sf::Vector2<T> rectDims(const sf::Rect<T> &rect) {
	return sf::Vector2<T>(rect.width, rect.height);
}

template <typename T>
constexpr inline sf::Vector2<T> rectLowerBound(const sf::Rect<T> &rect) {
	return sf::Vector2<T>(rect.left, rect.top);
}

template <typename T>
constexpr inline sf::Vector2<T> rectUpperBound(const sf::Rect<T> &rect) {
	return sf::Vector2<T>(rect.left + rect.width, rect.top + rect.height);
}

template <typename T>
constexpr inline sf::Rect<T> rectFromBounds(const sf::Vector2<T> &lowerBound, const sf::Vector2<T> &upperBound) {
	return sf::Rect<T>(lowerBound.x, lowerBound.y, upperBound.x - lowerBound.x, upperBound.y - lowerBound.y);
}

template <typename T>
constexpr inline float vectorMagnitude(const sf::Vector2<T> &vector) {
	return std::sqrt(vector.x * vector.x + vector.y * vector.y);
}

template <typename T>
constexpr inline float vectorMagnitudeSquared(const sf::Vector2<T> &vector) {
	return vector.x * vector.x + vector.y * vector.y;
}

template <typename T>
inline sf::Vector2<T> vectorNormalize(const sf::Vector2<T> &vector) {
	float magnitude = vectorMagnitude(vector);

	if (magnitude == 0.0f)
		return sf::Vector2<T>(1.0f, 0.0f);

	float distInv = 1.0f / magnitude;

	return sf::Vector2<T>(vector.x * distInv, vector.y * distInv);
}

template <typename T>
constexpr inline float vectorProject(const sf::Vector2<T> &left, const sf::Vector2<T> &right) {
	assert(vectorMagnitudeSquared(right) != 0.0f);

	return vectorDot(left, right) / vectorMagnitudeSquared(right);
}

template <typename T>
constexpr inline sf::Rect<T> rectRecenter(const sf::Rect<T> &rect, const sf::Vector2<T> &center) {
	return sf::Rect<T>(center - rectHalfDims(rect), rectDims(rect));
}

template <typename T>
constexpr inline float vectorDot(const sf::Vector2<T> &left, const sf::Vector2<T> &right) {
	return left.x * right.x + left.y * right.y;
}

template <typename T>
inline sf::Rect<T> rectExpand(const sf::Rect<T> &rect, const sf::Vector2<T> &point) {
	sf::Vector2<T> lowerBound = rectLowerBound(rect);
	sf::Vector2<T> upperBound = rectUpperBound(rect);

	if (point.x < lowerBound.x)
		lowerBound.x = point.x;
	else if (point.x > upperBound.x)
		upperBound.x = point.x;

	if (point.y < lowerBound.y)
		lowerBound.y = point.y;
	else if (point.y > upperBound.y)
		upperBound.y = point.y;

	return rectFromBounds(lowerBound, upperBound);
}

inline bool shapeIntersection(const sf::ConvexShape &left, const sf::ConvexShape &right) {
	std::vector<sf::Vector2f> transformedLeft(left.getPointCount());

	for (unsigned i = 0; i < left.getPointCount(); i++)
		transformedLeft[i] = left.getTransform().transformPoint(left.getPoint(i));

	std::vector<sf::Vector2f> transformedRight(right.getPointCount());

	for (unsigned i = 0; i < right.getPointCount(); i++)
		transformedRight[i] = right.getTransform().transformPoint(right.getPoint(i));

	for (unsigned i = 0; i < left.getPointCount(); i++) {
		sf::Vector2f point = transformedLeft[i];
		sf::Vector2f nextPoint;

		if (i == left.getPointCount() - 1u)
			nextPoint = transformedLeft[0];
		else
			nextPoint = transformedLeft[i + 1];

		sf::Vector2f edge = nextPoint - point;

		// Project points from other shape onto perpendicular
		sf::Vector2f edgePerpendicular = sf::Vector2f(edge.y, -edge.x);

		float pointProj = vectorProject(point, edgePerpendicular);

		float minRightProj = vectorProject(transformedRight[0], edgePerpendicular);

		for (unsigned j = 1; j < right.getPointCount(); j++) {
			float proj = vectorProject(transformedRight[j], edgePerpendicular);

			minRightProj = std::min(minRightProj, proj);
		}

		if (minRightProj > pointProj)
			return false;
	}

	for (unsigned i = 0; i < right.getPointCount(); i++) {
		sf::Vector2f point = transformedRight[i];
		sf::Vector2f nextPoint;

		if (i == right.getPointCount() - 1u)
			nextPoint = transformedRight[0];
		else
			nextPoint = transformedRight[i + 1];

		sf::Vector2f edge = nextPoint - point;

		// Project points from other shape onto perpendicular
		sf::Vector2f edgePerpendicular = sf::Vector2f(edge.y, -edge.x);

		float pointProj = vectorProject(point, edgePerpendicular);

		float minRightProj = vectorProject(transformedLeft[0], edgePerpendicular);

		for (unsigned j = 1; j < left.getPointCount(); j++) {
			float proj = vectorProject(transformedLeft[j], edgePerpendicular);

			minRightProj = std::min(minRightProj, proj);
		}

		if (minRightProj > pointProj)
			return false;
	}

	return true;
}

template <typename T>
inline sf::ConvexShape shapeFromRect(const sf::Rect<T> &rect) {
	sf::ConvexShape shape(4);

	sf::Vector2<T> halfDims = rectHalfDims(rect);

	shape.setPoint(0, { -halfDims.x, -halfDims.y });
	shape.setPoint(1, {  halfDims.x, -halfDims.y });
	shape.setPoint(2, {  halfDims.x,  halfDims.y });
	shape.setPoint(3, { -halfDims.x,  halfDims.y });

	shape.setPosition(rectCenter(rect));

	return shape;
}