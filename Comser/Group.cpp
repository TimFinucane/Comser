#include "Group.h"

#include <algorithm>

using namespace Comser;
using namespace Comser::Scene;

Group::Group( const std::initializer_list<ComponentType>& types )
    : _associator( types ), _components( types.size() ), _active( false )
{
    _addedSignals.resize( types.size() );
    _removedSignals.resize( types.size() );
}
Group::~Group()
{
    clear();

    _addedSignals.clear();
    _removedSignals.clear();
}

void        Group::clear()
{
    // Destroying in reverse order without iterators for safety
    for( size_t i = _entities.size(); i > 0; --i )
    {
        destroyEntity( (EntityId)(i - 1) );
    }
}

EntityId    Group::createEntity()
{
    return _entities.createEntity();
}
void        Group::destroyEntity( EntityId id )
{
    for( size_t i = _entities[id]->size(); i > 0; --i )
        _removeComponent( id, (_entities[id]->begin() + i - 1) );

    // Destroy the entity
    _entities.destroyEntity( id );
}

void        Group::_swap( EntityList::EntityIterator entityIt )
{
    // TODO: CHECK THIS WORKS MY MIND IS FUCK

    // Find the entities
    EntityId back = _components[entityIt->type]->get( (ComponentVector::Index)(_components[entityIt->type]->size() - 1) )->entity;

    // Now get the component definition for that given type
    EntityList::EntityIterator backIt = _entities.findComponent( back, entityIt->type );
    
    // Swap the two components in the vector
    _components[entityIt->type]->swap( backIt->index, entityIt->index );

    // Now swap the components in their entities componentDef
    std::iter_swap( backIt, entityIt );
}