#include "MultiGrid.h"

using namespace Comser;

void MultiGrid::destroyEntity( const Position& pos )
{
    Entity& ent = getEnt( pos );

    for( auto i = ent.begin(); i != ent.end(); ++i )
    {
        signalRemoved( i->type, pos, i->component );
    }
    ent.clear();
}