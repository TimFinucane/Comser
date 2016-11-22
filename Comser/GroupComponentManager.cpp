#include "GroupComponentManager.h"

#include <algorithm>

using namespace Comser::GroupSpace;

// Aaah, how i love the Standard Library

void    ComponentVector::pop()
{
    Component* cmp = _vector[_vector.size() - 1];

    delete cmp;

    _vector.pop_back();
}
void    ComponentVector::swap( Index a, Index b )
{
    std::iter_swap( (_vector.begin() + a), (_vector.begin() + b) );
}

Components::Components( size_t count )
{
    // TODO: Allocate
    _componentvectors = new ComponentVector[count];
}
Components::~Components()
{
    delete[] _componentvectors;
}
