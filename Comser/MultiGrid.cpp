#include "MultiGrid.h"

using namespace Comser;

Comser::WeakHandle MultiGrid::createEntity( const Position& pos )
{
    return std::make_shared<WeakEnt>( pos );
}
void MultiGrid::destroyEntity( Comser::WeakPtr entPtr )
{
    Entity& ent = getEnt( entPtr );

    for( auto i = ent.begin(); i != ent.end(); ++i )
    {
        signalRemoved( i->type, entPtr, i->component );
    }
    ent.clear();
}
void MultiGrid::destroyEntity( const Position& pos )
{
    const Comser::WeakPtr ptr = (void*)&pos;
    
    Entity& ent = getEnt( pos );

    for( auto i = ent.begin(); i != ent.end(); ++i )
    {
        signalRemoved( i->type, ptr, i->component );
    }
    ent.clear();
}