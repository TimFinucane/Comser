#include "Group.h"

#include <algorithm>

using namespace Comser;
using namespace Comser::Scene;

Group::Group( unsigned int count, ComponentType* types )
    : _associator( count, types ), _components( count )
{
    _addedSignals.reserve( count );
    _removedSignals.reserve( count );

    for( unsigned int i = 0; i < count; ++i )
    {
        _addedSignals.push_back( Signal() );
        _removedSignals.push_back( Signal() );
    }
}
Group::~Group()
{
    clear();

    for( unsigned int i = 0; i < _addedSignals.size(); ++i )
    {
        _addedSignals[i].clear();
        _removedSignals[i].clear();
    }
    _addedSignals.clear();
    _removedSignals.clear();
}

void        Group::clear()
{
    // Destroying in reverse order without iterators for safety
    for( int i = _entities.size(); i > 0; --i )
    {
        destroyEntity( i - 1 );
    }
}

EntityId    Group::createEntity()
{
    return _entities.createEntity();
}
void        Group::destroyEntity( EntityId id )
{
    for( int i = _entities[id]->size() - 1; i >= 0; --i )
    {
        // Destroy each component
        _removeComponent( id, (_entities[id]->begin() + i) );
    }

    // Destroy the entity
    _entities.destroyEntity( id );
}

sigc::connection    Group::connectAdded( LocalComponentType type, const Slot& slot )
{
    return _addedSignals[type.get()].connect( slot );
}
sigc::connection    Group::connectRemoved( LocalComponentType type, const Slot& slot )
{
    return _removedSignals[type.get()].connect( slot );
}


void        Group::_swap( LocalComponentType type, EntityId id, EntityList::EntityIterator entityIt )
{
    // Find the entity id of the back item
    EntityId back = _components[type]->get( (_components[type]->size() - 1) )->entity;

    // Now get the component associated with back
    EntityList::EntityIterator backIt = _entities.findComponent( back, type );
    
    // Swap the two components
    _components[type]->swap( backIt->index, entityIt->index );

    // Now swap the EntityIterators
    std::iter_swap( backIt, entityIt );
}