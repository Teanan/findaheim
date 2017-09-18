#pragma once

#include "quadtree/QuadtreeOccupant.h"

inline namespace ltbl
{
    //! A sprite reaction to lights.

    class NormalsSprite : public QuadtreeOccupant
    {
    public:

        NormalsSprite() {}

        inline sf::FloatRect getAABB() const
        {
            return _normalsSprite.getGlobalBounds();
        }

    public:

        sf::Sprite _normalsSprite;
    };
}
