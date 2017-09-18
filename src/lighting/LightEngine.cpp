#include "lighting/LightEngine.h"

#include <assert.h>

#include <iostream>



void LightEngine::getPenumbrasPoint(std::vector<Penumbra> &penumbras, std::vector<int> &innerBoundaryIndices, std::vector<sf::Vector2f> &innerBoundaryVectors, std::vector<int> &outerBoundaryIndices, std::vector<sf::Vector2f> &outerBoundaryVectors, const sf::ConvexShape &shape, const sf::Vector2f &sourceCenter, float sourceRadius) {
    const int numpoints = shape.getPointCount();

    std::vector<bool> bothedgesboundarywindings;
    bothedgesboundarywindings.reserve(2);

    std::vector<bool> oneedgeboundarywindings;
    oneedgeboundarywindings.reserve(2);

    // calculate front and back facing sides
    std::vector<bool> facingfrontbothedges;
    facingfrontbothedges.reserve(numpoints);

    std::vector<bool> facingfrontoneedge;
    facingfrontoneedge.reserve(numpoints);

    for (int i = 0; i < numpoints; i++) {
        sf::Vector2f point = shape.getTransform().transformPoint(shape.getPoint(i));

        sf::Vector2f nextpoint;

        if (i < numpoints - 1)
            nextpoint = shape.getTransform().transformPoint(shape.getPoint(i + 1));
        else
            nextpoint = shape.getTransform().transformPoint(shape.getPoint(0));
		
        sf::Vector2f firstedgeray;
        sf::Vector2f secondedgeray;
        sf::Vector2f firstnextedgeray;
        sf::Vector2f secondnextedgeray;

        {
            sf::Vector2f sourcetopoint = point - sourceCenter;

            sf::Vector2f perpendicularoffset(-sourcetopoint.y, sourcetopoint.x);

            perpendicularoffset = vectorNormalize(perpendicularoffset);
            perpendicularoffset *= sourceRadius;

            firstedgeray = point - (sourceCenter - perpendicularoffset);
            secondedgeray = point - (sourceCenter + perpendicularoffset);
        }

        {
            sf::Vector2f sourcetopoint = nextpoint - sourceCenter;

            sf::Vector2f perpendicularoffset(-sourcetopoint.y, sourcetopoint.x);

            perpendicularoffset = vectorNormalize(perpendicularoffset);
            perpendicularoffset *= sourceRadius;

            firstnextedgeray = nextpoint - (sourceCenter - perpendicularoffset);
            secondnextedgeray = nextpoint - (sourceCenter + perpendicularoffset);
        }

        sf::Vector2f pointtonextpoint = nextpoint - point;

        sf::Vector2f normal = vectorNormalize(sf::Vector2f(-pointtonextpoint.y, pointtonextpoint.x));

        // front facing, mark it
        facingfrontbothedges.push_back((vectorDot(firstedgeray, normal) > 0.0f && vectorDot(secondedgeray, normal) > 0.0f) || (vectorDot(firstnextedgeray, normal) > 0.0f && vectorDot(secondnextedgeray, normal) > 0.0f));
        facingfrontoneedge.push_back((vectorDot(firstedgeray, normal) > 0.0f || vectorDot(secondedgeray, normal) > 0.0f) || vectorDot(firstnextedgeray, normal) > 0.0f || vectorDot(secondnextedgeray, normal) > 0.0f);
    }

    // go through front/back facing list. where the facing direction switches, there is a boundary
    for (int i = 1; i < numpoints; i++)
        if (facingfrontbothedges[i] != facingfrontbothedges[i - 1]) {
            innerBoundaryIndices.push_back(i);
            bothedgesboundarywindings.push_back(facingfrontbothedges[i]);
        }

    // check looping indices separately
    if (facingfrontbothedges[0] != facingfrontbothedges[numpoints - 1]) {
        innerBoundaryIndices.push_back(0);
        bothedgesboundarywindings.push_back(facingfrontbothedges[0]);
    }

    // go through front/back facing list. where the facing direction switches, there is a boundary
    for (int i = 1; i < numpoints; i++)
        if (facingfrontoneedge[i] != facingfrontoneedge[i - 1]) {
            outerBoundaryIndices.push_back(i);
            oneedgeboundarywindings.push_back(facingfrontoneedge[i]);
        }

    // check looping indices separately
    if (facingfrontoneedge[0] != facingfrontoneedge[numpoints - 1]) {
        outerBoundaryIndices.push_back(0);
        oneedgeboundarywindings.push_back(facingfrontoneedge[0]);
    }

    // compute outer boundary vectors
    for (unsigned bi = 0; bi < outerBoundaryIndices.size(); bi++) {
        int penumbraindex = outerBoundaryIndices[bi];
        bool winding = oneedgeboundarywindings[bi];

        sf::Vector2f point = shape.getTransform().transformPoint(shape.getPoint(penumbraindex));

        sf::Vector2f sourcetopoint = point - sourceCenter;

        sf::Vector2f perpendicularoffset(-sourcetopoint.y, sourcetopoint.x);

        perpendicularoffset = vectorNormalize(perpendicularoffset);
        perpendicularoffset *= sourceRadius;

        sf::Vector2f firstedgeray = point - (sourceCenter + perpendicularoffset);
        sf::Vector2f secondedgeray = point - (sourceCenter - perpendicularoffset);

        // add boundary vector
        outerBoundaryVectors.push_back(winding ? firstedgeray : secondedgeray);
    }

    for (unsigned bi = 0; bi < innerBoundaryIndices.size(); bi++) {
        int penumbraindex = innerBoundaryIndices[bi];
        bool winding = bothedgesboundarywindings[bi];

        sf::Vector2f point = shape.getTransform().transformPoint(shape.getPoint(penumbraindex));

        sf::Vector2f sourcetopoint = point - sourceCenter;

        sf::Vector2f perpendicularoffset(-sourcetopoint.y, sourcetopoint.x);

        perpendicularoffset = vectorNormalize(perpendicularoffset);
        perpendicularoffset *= sourceRadius;

        sf::Vector2f firstedgeray = point - (sourceCenter + perpendicularoffset);
        sf::Vector2f secondedgeray = point - (sourceCenter - perpendicularoffset);

        // add boundary vector
        innerBoundaryVectors.push_back(winding ? secondedgeray : firstedgeray);
        sf::Vector2f outerboundaryvector = winding ? firstedgeray : secondedgeray;

        if (innerBoundaryIndices.size() == 1)
            innerBoundaryVectors.push_back(outerboundaryvector);

        // add penumbras
        bool hasprevpenumbra = false;

        sf::Vector2f prevpenumbralightedgevector;

        float prevbrightness = 1.0f;

        int counter = 0;

        while (penumbraindex != -1) {
            sf::Vector2f nextpoint;
            int nextpointindex;

            if (penumbraindex < numpoints - 1) {
                nextpointindex = penumbraindex + 1;
                nextpoint = shape.getTransform().transformPoint(shape.getPoint(penumbraindex + 1));
            }
            else {
                nextpointindex = 0;
                nextpoint = shape.getTransform().transformPoint(shape.getPoint(0));
            }

            sf::Vector2f pointtonextpoint = nextpoint - point;

            sf::Vector2f prevpoint;
            int prevpointindex;

            if (penumbraindex > 0) {
                prevpointindex = penumbraindex - 1;
                prevpoint = shape.getTransform().transformPoint(shape.getPoint(penumbraindex - 1));
            }
            else {
                prevpointindex = numpoints - 1;
                prevpoint = shape.getTransform().transformPoint(shape.getPoint(numpoints - 1));
            }

            sf::Vector2f pointtoprevpoint = prevpoint - point;

            LightEngine::Penumbra penumbra;

            penumbra._source = point;

            if (!winding) {
                if (hasprevpenumbra)
                    penumbra._lightEdge = prevpenumbralightedgevector;
                else
                    penumbra._lightEdge = innerBoundaryVectors.back();

                penumbra._darkEdge = outerboundaryvector;

                penumbra._lightBrightness = prevbrightness;

                // next point, check for intersection
                float intersectionangle = std::acos(vectorDot(vectorNormalize(penumbra._lightEdge), vectorNormalize(pointtonextpoint)));
                float penumbraangle = std::acos(vectorDot(vectorNormalize(penumbra._lightEdge), vectorNormalize(penumbra._darkEdge)));

                if (intersectionangle < penumbraangle) {
                    prevbrightness = penumbra._darkBrightness = intersectionangle / penumbraangle;

                    assert(prevbrightness >= 0.0f && prevbrightness <= 1.0f);

                    penumbra._darkEdge = pointtonextpoint;

                    penumbraindex = nextpointindex;

                    if (hasprevpenumbra) {
                        std::swap(penumbra._darkBrightness, penumbras.back()._darkBrightness);
                        std::swap(penumbra._lightBrightness, penumbras.back()._lightBrightness);
                    }

                    hasprevpenumbra = true;

                    prevpenumbralightedgevector = penumbra._darkEdge;

                    point = shape.getTransform().transformPoint(shape.getPoint(penumbraindex));

                    sourcetopoint = point - sourceCenter;

                    perpendicularoffset = sf::Vector2f(-sourcetopoint.y, sourcetopoint.x);

                    perpendicularoffset = vectorNormalize(perpendicularoffset);
                    perpendicularoffset *= sourceRadius;

                    firstedgeray = point - (sourceCenter + perpendicularoffset);
                    secondedgeray = point - (sourceCenter - perpendicularoffset);

                    outerboundaryvector = secondedgeray;

                    if (!outerBoundaryVectors.empty()) {
                        outerBoundaryVectors[0] = penumbra._darkEdge;
                        outerBoundaryIndices[0] = penumbraindex;
                    }
                }
                else {
                    penumbra._darkBrightness = 0.0f;

                    if (hasprevpenumbra) {
                        std::swap(penumbra._darkBrightness, penumbras.back()._darkBrightness);
                        std::swap(penumbra._lightBrightness, penumbras.back()._lightBrightness);
                    }

                    hasprevpenumbra = false;

                    if (!outerBoundaryVectors.empty()) {
                        outerBoundaryVectors[0] = penumbra._darkEdge;
                        outerBoundaryIndices[0] = penumbraindex;
                    }

                    penumbraindex = -1;
                }
            }
            else {
                if (hasprevpenumbra)
                    penumbra._lightEdge = prevpenumbralightedgevector;
                else
                    penumbra._lightEdge = innerBoundaryVectors.back();

                penumbra._darkEdge = outerboundaryvector;

                penumbra._lightBrightness = prevbrightness;

                // next point, check for intersection
                float intersectionangle = std::acos(vectorDot(vectorNormalize(penumbra._lightEdge), vectorNormalize(pointtoprevpoint)));
                float penumbraangle = std::acos(vectorDot(vectorNormalize(penumbra._lightEdge), vectorNormalize(penumbra._darkEdge)));

                if (intersectionangle < penumbraangle) {
                    prevbrightness = penumbra._darkBrightness = intersectionangle / penumbraangle;

                    assert(prevbrightness >= 0.0f && prevbrightness <= 1.0f);

                    penumbra._darkEdge = pointtoprevpoint;

                    penumbraindex = prevpointindex;

                    if (hasprevpenumbra) {
                        std::swap(penumbra._darkBrightness, penumbras.back()._darkBrightness);
                        std::swap(penumbra._lightBrightness, penumbras.back()._lightBrightness);
                    }

                    hasprevpenumbra = true;

                    prevpenumbralightedgevector = penumbra._darkEdge;

                    point = shape.getTransform().transformPoint(shape.getPoint(penumbraindex));

                    sourcetopoint = point - sourceCenter;

                    perpendicularoffset = sf::Vector2f(-sourcetopoint.y, sourcetopoint.x);

                    perpendicularoffset = vectorNormalize(perpendicularoffset);
                    perpendicularoffset *= sourceRadius;

                    firstedgeray = point - (sourceCenter + perpendicularoffset);
                    secondedgeray = point - (sourceCenter - perpendicularoffset);

                    outerboundaryvector = firstedgeray;

                    if (!outerBoundaryVectors.empty()) {
                        outerBoundaryVectors[1] = penumbra._darkEdge;
                        outerBoundaryIndices[1] = penumbraindex;
                    }
                }
                else {
                    penumbra._darkBrightness = 0.0f;

                    if (hasprevpenumbra) {
                        std::swap(penumbra._darkBrightness, penumbras.back()._darkBrightness);
                        std::swap(penumbra._lightBrightness, penumbras.back()._lightBrightness);
                    }

                    hasprevpenumbra = false;

                    if (!outerBoundaryVectors.empty()) {
                        outerBoundaryVectors[1] = penumbra._darkEdge;
                        outerBoundaryIndices[1] = penumbraindex;
                    }

                    penumbraindex = -1;
                }
            }

            penumbras.push_back(penumbra);

            counter++;
        }
    }
}

void LightEngine::getPenumbrasDirection(std::vector<Penumbra> &penumbras, std::vector<int> &innerBoundaryIndices, std::vector<sf::Vector2f> &innerBoundaryVectors, std::vector<int> &outerBoundaryIndices, std::vector<sf::Vector2f> &outerBoundaryVectors, const sf::ConvexShape &shape, const sf::Vector2f &sourceDirection, float sourceRadius, float sourceDistance) {
    const int numPoints = shape.getPointCount();

    innerBoundaryIndices.reserve(2);
    innerBoundaryVectors.reserve(2);
    penumbras.reserve(2);

    std::vector<bool> bothEdgesBoundaryWindings;
    bothEdgesBoundaryWindings.reserve(2);

    // Calculate front and back facing sides
    std::vector<bool> facingFrontBothEdges;
    facingFrontBothEdges.reserve(numPoints);

    std::vector<bool> facingFrontOneEdge;
    facingFrontOneEdge.reserve(numPoints);

    for (int i = 0; i < numPoints; i++) {
        sf::Vector2f point = shape.getTransform().transformPoint(shape.getPoint(i));

        sf::Vector2f nextPoint;

        if (i < numPoints - 1)
            nextPoint = shape.getTransform().transformPoint(shape.getPoint(i + 1));
        else
            nextPoint = shape.getTransform().transformPoint(shape.getPoint(0));

        sf::Vector2f firstEdgeRay;
        sf::Vector2f secondEdgeRay;
        sf::Vector2f firstNextEdgeRay;
        sf::Vector2f secondNextEdgeRay;

        sf::Vector2f perpendicularOffset(-sourceDirection.y, sourceDirection.x);

        perpendicularOffset = vectorNormalize(perpendicularOffset);
        perpendicularOffset *= sourceRadius;

        firstEdgeRay = point - (point - sourceDirection * sourceDistance - perpendicularOffset);
        secondEdgeRay = point - (point - sourceDirection * sourceDistance + perpendicularOffset);

        firstNextEdgeRay = nextPoint - (point - sourceDirection * sourceDistance - perpendicularOffset);
        secondNextEdgeRay = nextPoint - (point - sourceDirection * sourceDistance + perpendicularOffset);

        sf::Vector2f pointToNextPoint = nextPoint - point;

        sf::Vector2f normal = vectorNormalize(sf::Vector2f(-pointToNextPoint.y, pointToNextPoint.x));

        // Front facing, mark it
        facingFrontBothEdges.push_back((vectorDot(firstEdgeRay, normal) > 0.0f && vectorDot(secondEdgeRay, normal) > 0.0f) || (vectorDot(firstNextEdgeRay, normal) > 0.0f && vectorDot(secondNextEdgeRay, normal) > 0.0f));
        facingFrontOneEdge.push_back((vectorDot(firstEdgeRay, normal) > 0.0f || vectorDot(secondEdgeRay, normal) > 0.0f) || vectorDot(firstNextEdgeRay, normal) > 0.0f || vectorDot(secondNextEdgeRay, normal) > 0.0f);
    }

    // Go through front/back facing list. Where the facing direction switches, there is a boundary
    for (int i = 1; i < numPoints; i++)
        if (facingFrontBothEdges[i] != facingFrontBothEdges[i - 1]) {
            innerBoundaryIndices.push_back(i);
            bothEdgesBoundaryWindings.push_back(facingFrontBothEdges[i]);
        }

    // Check looping indices separately
    if (facingFrontBothEdges[0] != facingFrontBothEdges[numPoints - 1]) {
        innerBoundaryIndices.push_back(0);
        bothEdgesBoundaryWindings.push_back(facingFrontBothEdges[0]);
    }

    // Go through front/back facing list. Where the facing direction switches, there is a boundary
    for (int i = 1; i < numPoints; i++)
        if (facingFrontOneEdge[i] != facingFrontOneEdge[i - 1])
            outerBoundaryIndices.push_back(i);

    // Check looping indices separately
    if (facingFrontOneEdge[0] != facingFrontOneEdge[numPoints - 1])
        outerBoundaryIndices.push_back(0);

    for (unsigned bi = 0; bi < innerBoundaryIndices.size(); bi++) {
        int penumbraIndex = innerBoundaryIndices[bi];
        bool winding = bothEdgesBoundaryWindings[bi];

        sf::Vector2f point = shape.getTransform().transformPoint(shape.getPoint(penumbraIndex));

        sf::Vector2f perpendicularOffset(-sourceDirection.y, sourceDirection.x);

        perpendicularOffset = vectorNormalize(perpendicularOffset);
        perpendicularOffset *= sourceRadius;

        sf::Vector2f firstEdgeRay = point - (point - sourceDirection * sourceDistance + perpendicularOffset);
        sf::Vector2f secondEdgeRay = point - (point - sourceDirection * sourceDistance - perpendicularOffset);

        // Add boundary vector
        innerBoundaryVectors.push_back(winding ? secondEdgeRay : firstEdgeRay);
        sf::Vector2f outerBoundaryVector = winding ? firstEdgeRay : secondEdgeRay;

        outerBoundaryVectors.push_back(outerBoundaryVector);

        // Add penumbras
        bool hasPrevPenumbra = false;

        sf::Vector2f prevPenumbraLightEdgeVector;

        float prevBrightness = 1.0f;

        int counter = 0;

        while (penumbraIndex != -1) {
            sf::Vector2f nextPoint;
            int nextPointIndex;

            if (penumbraIndex < numPoints - 1) {
                nextPointIndex = penumbraIndex + 1;
                nextPoint = shape.getTransform().transformPoint(shape.getPoint(penumbraIndex + 1));
            }
            else {
                nextPointIndex = 0;
                nextPoint = shape.getTransform().transformPoint(shape.getPoint(0));
            }

            sf::Vector2f pointToNextPoint = nextPoint - point;

            sf::Vector2f prevPoint;
            int prevPointIndex;

            if (penumbraIndex > 0) {
                prevPointIndex = penumbraIndex - 1;
                prevPoint = shape.getTransform().transformPoint(shape.getPoint(penumbraIndex - 1));
            }
            else {
                prevPointIndex = numPoints - 1;
                prevPoint = shape.getTransform().transformPoint(shape.getPoint(numPoints - 1));
            }

            sf::Vector2f pointToPrevPoint = prevPoint - point;

            LightEngine::Penumbra penumbra;

            penumbra._source = point;

            if (!winding) {
                if (hasPrevPenumbra)
                    penumbra._lightEdge = prevPenumbraLightEdgeVector;
                else
                    penumbra._lightEdge = innerBoundaryVectors.back();

                penumbra._darkEdge = outerBoundaryVector;

                penumbra._lightBrightness = prevBrightness;

                // Next point, check for intersection
                float intersectionAngle = std::acos(vectorDot(vectorNormalize(penumbra._lightEdge), vectorNormalize(pointToNextPoint)));
                float penumbraAngle = std::acos(vectorDot(vectorNormalize(penumbra._lightEdge), vectorNormalize(penumbra._darkEdge)));

                if (intersectionAngle < penumbraAngle) {
                    prevBrightness = penumbra._darkBrightness = intersectionAngle / penumbraAngle;

                    assert(prevBrightness >= 0.0f && prevBrightness <= 1.0f);

                    penumbra._darkEdge = pointToNextPoint;

                    penumbraIndex = nextPointIndex;

                    if (hasPrevPenumbra) {
                        std::swap(penumbra._darkBrightness, penumbras.back()._darkBrightness);
                        std::swap(penumbra._lightBrightness, penumbras.back()._lightBrightness);
                    }

                    hasPrevPenumbra = true;

                    prevPenumbraLightEdgeVector = penumbra._darkEdge;

                    point = shape.getTransform().transformPoint(shape.getPoint(penumbraIndex));

                    perpendicularOffset = sf::Vector2f(-sourceDirection.y, sourceDirection.x);

                    perpendicularOffset = vectorNormalize(perpendicularOffset);
                    perpendicularOffset *= sourceRadius;

                    firstEdgeRay = point - (point - sourceDirection * sourceDistance + perpendicularOffset);
                    secondEdgeRay = point - (point - sourceDirection * sourceDistance - perpendicularOffset);

                    outerBoundaryVector = secondEdgeRay;
                }
                else {
                    penumbra._darkBrightness = 0.0f;

                    if (hasPrevPenumbra) {
                        std::swap(penumbra._darkBrightness, penumbras.back()._darkBrightness);
                        std::swap(penumbra._lightBrightness, penumbras.back()._lightBrightness);
                    }

                    hasPrevPenumbra = false;

                    penumbraIndex = -1;
                }
            }
            else {
                if (hasPrevPenumbra)
                    penumbra._lightEdge = prevPenumbraLightEdgeVector;
                else
                    penumbra._lightEdge = innerBoundaryVectors.back();

                penumbra._darkEdge = outerBoundaryVector;

                penumbra._lightBrightness = prevBrightness;

                // Next point, check for intersection
                float intersectionAngle = std::acos(vectorDot(vectorNormalize(penumbra._lightEdge), vectorNormalize(pointToPrevPoint)));
                float penumbraAngle = std::acos(vectorDot(vectorNormalize(penumbra._lightEdge), vectorNormalize(penumbra._darkEdge)));

                if (intersectionAngle < penumbraAngle) {
                    prevBrightness = penumbra._darkBrightness = intersectionAngle / penumbraAngle;

                    assert(prevBrightness >= 0.0f && prevBrightness <= 1.0f);

                    penumbra._darkEdge = pointToPrevPoint;

                    penumbraIndex = prevPointIndex;

                    if (hasPrevPenumbra) {
                        std::swap(penumbra._darkBrightness, penumbras.back()._darkBrightness);
                        std::swap(penumbra._lightBrightness, penumbras.back()._lightBrightness);
                    }

                    hasPrevPenumbra = true;

                    prevPenumbraLightEdgeVector = penumbra._darkEdge;

                    point = shape.getTransform().transformPoint(shape.getPoint(penumbraIndex));

                    perpendicularOffset = sf::Vector2f(-sourceDirection.y, sourceDirection.x);

                    perpendicularOffset = vectorNormalize(perpendicularOffset);
                    perpendicularOffset *= sourceRadius;

                    firstEdgeRay = point - (point - sourceDirection * sourceDistance + perpendicularOffset);
                    secondEdgeRay = point - (point - sourceDirection * sourceDistance - perpendicularOffset);

                    outerBoundaryVector = firstEdgeRay;
                }
                else {
                    penumbra._darkBrightness = 0.0f;

                    if (hasPrevPenumbra) {
                        std::swap(penumbra._darkBrightness, penumbras.back()._darkBrightness);
                        std::swap(penumbra._lightBrightness, penumbras.back()._lightBrightness);
                    }

                    hasPrevPenumbra = false;

                    penumbraIndex = -1;
                }
            }

            penumbras.push_back(penumbra);

            counter++;
        }
    }
}

void LightEngine::create(const sf::FloatRect& rootRegion, const sf::Vector2u& imageSize,
                         const sf::Texture& penumbraTexture,
                         sf::Shader& unshadowShader, sf::Shader& lightOverShapeShader, sf::Shader& normalsShader)
{
    _shapeQuadtree.create(rootRegion);
    _lightPointEmissionQuadtree.create(rootRegion);

    _lightTempTexture.create(imageSize.x, imageSize.y);
    _emissionTempTexture.create(imageSize.x, imageSize.y);
    _antumbraTempTexture.create(imageSize.x, imageSize.y);
    _compositionTexture.create(imageSize.x, imageSize.y);
    _normalsTexture.create(imageSize.x, imageSize.y);

    normalsTargetClear();

    sf::Vector2f targetSizeInv = sf::Vector2f(1.0f / imageSize.x, 1.0f / imageSize.y);

    unshadowShader.setParameter("penumbraTexture", penumbraTexture);

    lightOverShapeShader.setParameter("emissionTexture", _emissionTempTexture.getTexture());
    lightOverShapeShader.setParameter("targetSizeInv", targetSizeInv);

    normalsShader.setParameter("normalsTexture", _normalsTexture.getTexture());
    normalsShader.setParameter("targetSize", imageSize.x, imageSize.y);
    normalsShader.setParameter("lightTexture", sf::Shader::CurrentTexture);
}

void LightEngine::render(const sf::View &view, sf::Shader &unshadowShader, sf::Shader &lightOverShapeShader, sf::Shader& normalsShader)
{
    _compositionTexture.clear(_ambientColor);
    _compositionTexture.setView(_compositionTexture.getDefaultView());

    // Get bounding rectangle of view
	sf::FloatRect viewBounds = sf::FloatRect(view.getCenter().x, view.getCenter().y, 0.0f, 0.0f);
    viewBounds = rectExpand(viewBounds, _lightTempTexture.mapPixelToCoords(sf::Vector2i(0, 0)));
    viewBounds = rectExpand(viewBounds, _lightTempTexture.mapPixelToCoords(sf::Vector2i(_lightTempTexture.getSize().x, 0)));
    viewBounds = rectExpand(viewBounds, _lightTempTexture.mapPixelToCoords(sf::Vector2i(_lightTempTexture.getSize().x, _lightTempTexture.getSize().y)));
    viewBounds = rectExpand(viewBounds, _lightTempTexture.mapPixelToCoords(sf::Vector2i(0, _lightTempTexture.getSize().y)));

    sf::FloatRect centeredViewBounds = rectRecenter(viewBounds, sf::Vector2f(0.0f, 0.0f));

    _lightTempTexture.setView(view);

    std::vector<QuadtreeOccupant*> viewPointEmissionLights;

    _lightPointEmissionQuadtree.queryRegion(viewPointEmissionLights, viewBounds);

    sf::RenderStates compoRenderStates;
    compoRenderStates.blendMode = sf::BlendAdd;

    //----- Point lights

    std::vector<QuadtreeOccupant*> lightShapes;
    sf::Sprite lightTempSprite(_lightTempTexture.getTexture());

    for (auto occupant : viewPointEmissionLights) {
        auto pPointEmissionLight = static_cast<LightPointEmission*>(occupant);

        // Query shapes this light is affected by
        lightShapes.clear();
        _shapeQuadtree.queryRegion(lightShapes, pPointEmissionLight->getAABB());

        pPointEmissionLight->render(view, _lightTempTexture, _emissionTempTexture, _antumbraTempTexture, lightShapes, unshadowShader, lightOverShapeShader, _normalsEnabled, normalsShader);
        _compositionTexture.draw(lightTempSprite, compoRenderStates);
    }

    //----- Direction lights

    for (const auto& directionEmissionLight : _directionEmissionLights) {
        LightDirectionEmission* pDirectionEmissionLight = directionEmissionLight.get();

        float maxDim = std::max(centeredViewBounds.width, centeredViewBounds.height);
        sf::FloatRect extendedViewBounds = rectFromBounds(sf::Vector2f(-maxDim, -maxDim) * _directionEmissionRadiusMultiplier,
                                                          sf::Vector2f(maxDim, maxDim) * _directionEmissionRadiusMultiplier + sf::Vector2f(_directionEmissionRange, 0.0f));
        float shadowExtension = vectorMagnitude(rectLowerBound(centeredViewBounds)) * _directionEmissionRadiusMultiplier * 2.0f;

        sf::ConvexShape directionShape = shapeFromRect(extendedViewBounds);
        directionShape.setPosition(view.getCenter());

        sf::Vector2f normalizedCastDirection = vectorNormalize(pDirectionEmissionLight->_castDirection);
        directionShape.setRotation(_radToDeg * std::atan2(normalizedCastDirection.y, normalizedCastDirection.x));

        std::vector<QuadtreeOccupant*> viewLightShapes;
        _shapeQuadtree.queryShape(viewLightShapes, directionShape);

        pDirectionEmissionLight->render(view, _lightTempTexture, _antumbraTempTexture, viewLightShapes, unshadowShader, shadowExtension);

        sf::Sprite sprite;
        sprite.setTexture(_lightTempTexture.getTexture());
        _compositionTexture.draw(sprite, compoRenderStates);

        // TODO Normals
    }

    _compositionTexture.display();
}

LightShape* LightEngine::allocateShape()
{
    return _lightShapesPool.newElement();
}

void LightEngine::deallocateShape(LightShape* pLightShape)
{
    _lightShapesPool.deleteElement(pLightShape);
}

void LightEngine::addShape(LightShape* pLightShape)
{
    _shapeQuadtree.add(pLightShape);
}

void LightEngine::removeShape(LightShape* pLightShape)
{
    pLightShape->quadtreeRemove();
}

void LightEngine::addLight(const std::shared_ptr<LightPointEmission> &pointEmissionLight) {
    _lightPointEmissionQuadtree.add(pointEmissionLight.get());
    _pointEmissionLights.insert(pointEmissionLight);
}

void LightEngine::addLight(const std::shared_ptr<LightDirectionEmission> &directionEmissionLight) {
    _directionEmissionLights.insert(directionEmissionLight);
}

void LightEngine::removeLight(const std::shared_ptr<LightPointEmission> &pointEmissionLight) {
    std::unordered_set<std::shared_ptr<LightPointEmission>>::iterator it = _pointEmissionLights.find(pointEmissionLight);

    if (it != _pointEmissionLights.end()) {
        (*it)->quadtreeRemove();

        _pointEmissionLights.erase(it);
    }
}

void LightEngine::removeLight(const std::shared_ptr<LightDirectionEmission> &directionEmissionLight) {
    std::unordered_set<std::shared_ptr<LightDirectionEmission>>::iterator it = _directionEmissionLights.find(directionEmissionLight);
    if (it != _directionEmissionLights.end())
        _directionEmissionLights.erase(it);
}

//----- Normals -----//

void LightEngine::normalsEnabled(bool enabled)
{
    _normalsEnabled = enabled;
}

void LightEngine::normalsTargetSetView(sf::View view)
{
    _normalsTexture.setView(view);
}

void LightEngine::normalsTargetClear()
{
    _normalsTexture.clear(sf::Color{127u, 127u, 255u});
}

void LightEngine::normalsTargetDisplay()
{
    _normalsTexture.display();
}

void LightEngine::normalsTargetDraw(const sf::Drawable& drawable, sf::RenderStates states)
{
    _normalsTexture.draw(drawable, states);
}
