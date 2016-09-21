#include "ComponentManager.h"

#include <algorithm>

using namespace Comser;
using namespace Comser::Scene;

// Aaah, how i love the Standard Library

void    ComponentVector::pop()
{
    Component* cmp = _vector[_vector.size() - 1].component;

    delete cmp;

    _vector.pop_back();
}
void    ComponentVector::swap( Index a, Index b )
{
    std::swap( _vector.begin() + a, _vector.begin() + b );
}

ComponentAssociator::ComponentAssociator( unsigned int count, ComponentType* types )
    : _count( count )
{
    // TODO: Allocation
    _types = new ComponentType[count];

    // Move types to _types
    memcpy( _types, types, count * sizeof(&types) );
    
    std::sort( _types, &_types[count] );
}
LocalComponentType  ComponentAssociator::operator[]( ComponentType type ) const
{
    ComponentType* pos = std::lower_bound( _types, &_types[_count], type );
    return LocalComponentType( std::distance( _types, pos ) );
}

Components::Components( unsigned int count )
{
    // TODO: Allocate
    _componentvectors = new ComponentVector[count];
}

