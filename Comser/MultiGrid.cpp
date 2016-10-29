#include "MultiGrid.h"

using namespace Comser::MultiGrid;

Comser::EntityHandle Scene::createEntity( const Position& pos )
{
    return std::make_shared<Handle>( pos, _positionChange );
}
void Scene::destroyEntity( EntityHandle& handle )
{
    Entity& ent = getEnt( handle );

    for( auto i = ent.begin(); i != ent.end(); ++i )
    {
        signalRemoved( i->type, handle, i->component );
    }
    ent.clear();
}
void Scene::destroyEntity( const Position& pos )
{
    EntityHandle handle = createEntity( pos );

    Entity& ent = getEnt( pos );

    for( auto i = ent.begin(); i != ent.end(); ++i )
    {
        signalRemoved( i->type, handle, i->component );
    }
    ent.clear();
}