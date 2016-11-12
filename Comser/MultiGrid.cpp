#include "MultiGrid.h"

using namespace Comser::MultiGrid;

Comser::WeakHandle Scene::createEntity( const Position& pos )
{
    return std::make_shared<WeakEnt>( pos );
}
void Scene::destroyEntity( Comser::WeakPtr entPtr )
{
    Entity& ent = getEnt( entPtr );

    for( auto i = ent.begin(); i != ent.end(); ++i )
    {
        signalRemoved( i->type, entPtr, i->component );
    }
    ent.clear();
}
void Scene::destroyEntity( const Position& pos )
{
    const Comser::WeakPtr ptr = (void*)&pos;
    
    Entity& ent = getEnt( pos );

    for( auto i = ent.begin(); i != ent.end(); ++i )
    {
        signalRemoved( i->type, ptr, i->component );
    }
    ent.clear();
}