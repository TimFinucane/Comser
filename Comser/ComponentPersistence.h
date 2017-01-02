#pragma once

#include <iostream>

#include <functional>
#include <vector>
#include <algorithm>

#include "Component.h"
#include "ComponentAssociator.h"

namespace Comser::IO
{
    class ComponentPersistence
    {
    public:
        // TODO: Loader returns pointer to component?
        template <typename COMPONENT>
        using Loader = std::function<COMPONENT*( std::istream&, void* )>;
        template <typename COMPONENT>
        using Saver = std::function<void( std::ostream&, const COMPONENT* )>;

    public:
        template <typename COMPONENT>
        void    hook( Comser::LocalComponentType ltype, typename Loader<COMPONENT> loader, typename Saver<COMPONENT> saver )
        {
            // Thanks internet for showing no possible way of typecasting std::function, so now I have to do this.
            ComponentIO comp{ COMPONENT::id(), ltype, sizeof( COMPONENT ), loader, *reinterpret_cast<GenSaver*>( &saver ) };

            add( comp );
        }

        template <typename COMPONENT>
        void    remove();

    protected:
        using GenLoader = std::function<Comser::Component*( std::istream&, void* )>;
        using GenSaver = std::function<void( std::ostream&, const Comser::Component* )>;
        struct ComponentIO
        {
            Comser::ComponentType       type;
            Comser::LocalComponentType  localType;
            size_t                      size;
            GenLoader                   loader;
            GenSaver                    saver;

            bool operator <( Comser::ComponentType ctype )
            {
                return this->type < ctype;
            }
        };

        const ComponentIO*    findInfo( Comser::ComponentType type );

        std::vector<ComponentIO>    ioItems;

    private:
        void                add( const ComponentIO& comp );
    };
}