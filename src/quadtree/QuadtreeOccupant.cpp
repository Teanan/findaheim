#include "quadtree/QuadtreeOccupant.h"
#include "quadtree/QuadtreeNode.h"
#include "quadtree/Quadtree.h"

#include <assert.h>



void QuadtreeOccupant::quadtreeUpdate() {
    if (_pQuadtreeNode != nullptr)
        _pQuadtreeNode->update(this);
    else {
        _pQuadtree->_outsideRoot.erase(this);

        _pQuadtree->add(this);
    }
}

void QuadtreeOccupant::quadtreeRemove() {
    if (_pQuadtreeNode != nullptr)
        _pQuadtreeNode->remove(this);
    else
        _pQuadtree->_outsideRoot.erase(this);
}
