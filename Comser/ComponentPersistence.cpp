#include "ComponentPersistence.h"

#include <algorithm>

using namespace Comser::IO;

const ComponentPersistence::ComponentIO*  ComponentPersistence::findInfo( Comser::ComponentType type )
{
    auto it = std::lower_bound( ioItems.begin(), ioItems.end(), type );

    return (it == ioItems.end() || it->type != type) ? nullptr : &*it;
}

void ComponentPersistence::add( const ComponentIO& comp )
{
    ioItems.insert( std::lower_bound( ioItems.begin(), ioItems.end(), comp.type ), comp );
}