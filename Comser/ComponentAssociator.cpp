#include "ComponentAssociator.h"

#include <algorithm>

using namespace Comser;

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
    if( *pos != type )
        return NOT_FOUND();
    return LocalComponentType( (unsigned short)std::distance( _types, pos ) );
}