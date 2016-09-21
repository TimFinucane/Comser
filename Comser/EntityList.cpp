#include "EntityList.h"

#include <algorithm>

using namespace Comser;
using namespace Comser::Scene;

EntityId        EntityList::createEntity()
{
    // Now add the item to the vector
    // Find an available entity slot
    EntityId entity;
    if( _emptySlots.empty() )
    {
        entity = (EntityId)_entities.size();
        _entities.push_back( Entity() );
    }
    else
    {
        // Set entity to last element in emptySlots
        entity = *(_emptySlots.end() - 1);
        _emptySlots.pop_back();

        _entities[entity] = Entity();
    }

    return entity;
}
void            EntityList::destroyEntity( EntityId id )
{
    _entities[id].clear();
    // So we don't leave an unnecessary amount of unused space.
    _entities.shrink_to_fit();

    _emptySlots.push_back( id );
}

void            EntityList::addComponent( EntityId id, LocalComponentType type, ComponentVector::Index index )
{
    Entity::iterator it = findComponent( id, type );
    _entities[id].insert( it, { type, index } );
}
void            EntityList::removeComponent( EntityId id, LocalComponentType type )
{
    // TODO: Error checking?
    _entities[id].erase( findComponent( id, type ) );
}
void            EntityList::removeComponent( EntityId id, EntityIterator iterator )
{
    _entities[id].erase( iterator );
}

EntityList::EntityIterator  EntityList::findComponent( EntityId id, LocalComponentType type )
{
    // TODO: Error checking?
    auto pred = []( const ComponentDef& component, const LocalComponentType& constant )
    {
        return (component.type < constant);
    };
    
    EntityIterator it = std::lower_bound( _entities[id].begin(), _entities[id].end(), type, pred );

    return it;
}