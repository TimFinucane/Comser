#include "Group.h"

#include <algorithm>

using namespace Comser;

Group::Group( const std::initializer_list<Comser::ComponentType>& types )
    : _components( types.size() ), Comser::Scene<EntityList::EntityId>( types )
{
}
Group::~Group()
{
    clear();
}

void        Group::clear()
{
    // Destroying in reverse order without iterators for safety
    for( size_t i = _entities.size(); i > 0; --i )
    {
        for( size_t j = _entities[(unsigned int)i-1]->size(); j > 0; --j )
            _removeComponent( (unsigned int)i - 1, (_entities[(unsigned int)i - 1]->begin() + (unsigned int)j - 1) );

        // Destroy the entity
        _entities.destroyEntity( (unsigned int)i - 1 );
    }
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
    EntityList::ComponentDef search;
    search.index = (unsigned int)_components[entityIt->type]->size() - 1;
    search.type = entityIt->type;

    // Find and replace the component index of the entity owning the last component with
    //  the new position in which that component will soon be.
    for( auto it = _entities.begin(); it != _entities.end(); ++it )
    {
        auto compDef = std::find( it->begin(), it->end(), search );
        if( compDef != it->end() )
        {
            compDef->index = entityIt->index;
            break;
        }
    };

    // Swap the two components in the vector
    _components[entityIt->type]->swap( search.index, entityIt->index );

    // And set the entity index to the last component
    entityIt->index = search.index;
}