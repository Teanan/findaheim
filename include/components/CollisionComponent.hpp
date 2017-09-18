#ifndef ANAX_EXAMPLES_COMMON_COMPONENTS_COLLIDERCOMPONENT_HPP
#define ANAX_EXAMPLES_COMMON_COMPONENTS_COLLIDERCOMPONENT_HPP

#include <anax/Component.hpp>

#include <SFML/Graphics/Rect.hpp>

/// \brief A component used to store collision information
///
/// \author Miguel Martin
struct CollisionComponent : anax::Component
{
    /// The BBox rectangle for collision
    sf::FloatRect boundingBox;

    /// A dirty flag to determine whether this object causes events or not
    bool causesEvents;
};

#endif // ANAX_EXAMPLES_COMMON_COMPONENTS_COLLIDERCOMPONENT_HPP
