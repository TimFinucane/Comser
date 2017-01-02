#include "GridPersistence.h"

using namespace Comser::IO;

#define readvar( x ) read( reinterpret_cast<char*>( &x ), sizeof( x ) )
#define writevar( x ) write( reinterpret_cast<const char*>( &x ), sizeof( x ) )

void    GridPersistence::load( std::istream& in )
{
    // First are the dimensions. TODO: Something with this
    {
        unsigned int width;
        in.readvar( width );
    }
    {
        unsigned int height;
        in.readvar( height );
    }
    {
        unsigned int depth;
        in.readvar( depth );
    }

    // Now read the total number of objects in the grid
    unsigned int items;
    in.readvar( items );

    for( unsigned int i = 0; i < items; ++i )
    {
        Comser::MultiGridPosition pos;
        in.readvar( pos );

        unsigned int components;
        in.readvar( components );

        for( unsigned int j = 0; j < components; ++j )
        {
            Comser::ComponentType componentType;
            in.readvar( componentType );

            // Find the component type
            const auto& info = *findInfo( componentType );

            // Construct the component in-place. Wooooooooo that means no templating on component needed here
            auto placementConstructor = std::bind( info.loader, std::ref( in ), std::placeholders::_1 );
            _grid.addComponent( pos, info.localType, info.size, placementConstructor );
        }
    }
}

void    GridPersistence::save( std::ostream& out )
{
    // First write dimensions
    {
        unsigned int width = _grid.width();
        out.writevar( width );
    }
    {
        unsigned int height = _grid.height();
        out.writevar( height );
    }
    {
        unsigned int depth = _grid.depth();
        out.writevar( depth );
    }

    // Keep a place here to write the number of grid items
    auto tileCountPos = out.tellp();
    unsigned int tileCount = 0;
    out.writevar( tileCount );

    // Now write our tiles
    for( unsigned int y = 0; y < _grid.height(); ++y )
        for( unsigned int x = 0; x < _grid.width(); ++x )
            for( unsigned int z = 0; z < _grid.depth(); ++z )
            {
                Comser::MultiGridPosition pos = { x, y, z };

                if( !_grid.entityExists( pos ) )
                    continue;

                tileCount++;

                out.writevar( pos );

                auto componentsCountPos = out.tellp();
                unsigned int components = 0;
                out.writevar( components );

                // TODO: Revisit for some XTREME optimisation
                // Find all existing components and save them
                for( auto info = ioItems.begin(); info != ioItems.end(); ++info )
                    if( Comser::Component* comp = _grid.getComponent( pos, info->localType ) )
                    {
                        out.writevar( info->type );

                        info->saver( out, comp );
                        components++;
                    }

                // Write our component count then return to end
                auto streamPos = out.tellp();
                out.seekp( componentsCountPos );
                out.writevar( components );
                out.seekp( streamPos );
            }

    // Now revisit the tilecount position with our known number
    auto streamPos = out.tellp();
    out.seekp( tileCountPos );
    out.writevar( tileCount );
    out.seekp( streamPos );
}