#pragma once

#include <iostream>

#include "ComponentPersistence.h"
#include "Group.h"

namespace Comser::IO
{
    // TODO: Create components using placement new?
    class GroupPersistence : public ComponentPersistence
    {
    public:
        GroupPersistence( Comser::Group& group )
            : _group( group )
        {
        }

        template <typename COMPONENT>
        void    hook( typename Loader<COMPONENT> loader, typename Saver<COMPONENT> saver )
        {
            ComponentPersistence::hook<COMPONENT>( _group[COMPONENT::id()], loader, saver );
        }

        void    load( std::istream& in );
        void    save( std::ostream& out );

    private:
        Comser::Group&          _group;
    };

}