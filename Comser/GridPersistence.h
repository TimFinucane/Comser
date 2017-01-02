#pragma once

#include <iostream>

#include "ComponentPersistence.h"
#include "MultiGrid.h"

namespace Comser::IO
{
    // TODO: Create components using placement new?
    class GridPersistence : public ComponentPersistence
    {
    public:
        GridPersistence( Comser::MultiGrid& grid )
            : _grid( grid )
        {
        }

        template <typename COMPONENT>
        void    hook( typename Loader<COMPONENT> loader, typename Saver<COMPONENT> saver )
        {
            ComponentPersistence::hook<COMPONENT>( _grid[COMPONENT::id()], loader, saver );
        }

        void    load( std::istream& in );
        void    save( std::ostream& out );

    private:
        Comser::MultiGrid&          _grid;
    };

}