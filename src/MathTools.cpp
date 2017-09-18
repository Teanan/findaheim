#include <MathTools.h>

#include <list>

#include <assert.h>

#include <cmath>


sf::ConvexShape shapeFixWinding(const sf::ConvexShape &shape) {
    sf::Vector2f center = sf::Vector2f(0.0f, 0.0f);
    std::list<sf::Vector2f> points;

    for (unsigned i = 0; i < shape.getPointCount(); i++) {
        points.push_back(shape.getPoint(i));
        center += shape.getPoint(i);
    }

    center /= static_cast<float>(shape.getPointCount());

    // Fix winding
    sf::Vector2f lastPoint = points.front();
    points.pop_front();

    std::vector<sf::Vector2f> fixedPoints;

    fixedPoints.push_back(lastPoint);

    while (fixedPoints.size() < shape.getPointCount()) {
        sf::Vector2f centerToLastPoint = lastPoint - center;
        sf::Vector2f lastPointDirection = vectorNormalize(sf::Vector2f(-centerToLastPoint.y, centerToLastPoint.x));

        float maxD = -999999.0f;

        std::list<sf::Vector2f>::iterator nextPointIt;

        // Get next point
        for (std::list<sf::Vector2f>::iterator it = points.begin(); it != points.end(); it++) {
            sf::Vector2f toPointNormalized = vectorNormalize(*it - lastPoint);

            float d = vectorDot(toPointNormalized, lastPointDirection);

            if (d > maxD) {
                maxD = d;
                nextPointIt = it;
            }
        }

        fixedPoints.push_back(*nextPointIt);

        points.erase(nextPointIt);
    }

    sf::ConvexShape fixedShape(shape.getPointCount());

    for (unsigned i = 0; i < shape.getPointCount(); i++)
        fixedShape.setPoint(i, fixedPoints[i]);

    return fixedShape;
}

bool rayIntersect(const sf::Vector2f &as, const sf::Vector2f &ad, const sf::Vector2f &bs, const sf::Vector2f &bd, sf::Vector2f &intersection) {
    float dx = bs.x - as.x;
    float dy = bs.y - as.y;
    float det = bd.x * ad.y - bd.y * ad.x;

    if (det == 0.0f)
        return false;

    float u = (dy * bd.x - dx * bd.y) / det;

    if (u < 0.0f)
        return false;

    float v = (dy * ad.x - dx * ad.y) / det;

    if (v < 0.0f)
        return false;

    intersection = as + ad * u;

    return true;
}
