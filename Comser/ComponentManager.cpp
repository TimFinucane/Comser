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
    std::iter_swap( (_vector.begin() + a), (_vector.begin() + b) );
}

ComponentAssociator::ComponentAssociator( const std::initializer_list<ComponentType>& types )
    : _count( types.size() )
{
    // TODO: Allocation
    _types = new ComponentType[_count];

    // Move types to _types
    std::copy( types.begin(), types.end(), _types );
    
    std::sort( _types, &_types[_count] );
}
LocalComponentType  ComponentAssociator::operator[]( ComponentType type ) const
{
    ComponentType* pos = std::lower_bound( _types, &_types[_count], type );
    return LocalComponentType( (unsigned short)std::distance( _types, pos ) );
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
