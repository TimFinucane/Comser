#include "GroupPersistence.h"

using namespace Comser::IO;

#define readvar( x ) read( reinterpret_cast<char*>( &x ), sizeof( x ) )
#define writevar( x ) write( reinterpret_cast<const char*>( &x ), sizeof( x ) )

void    GroupPersistence::load( std::istream& in )
{
    // Now read the total number of objects in the group
    size_t items;
    in.readvar( items );

    for( unsigned int i = 0; i < items; ++i )
    {
        Comser::Group::EntityId entityId = _group.createEntity();

        unsigned int components;
        in.readvar( components );

        for( unsigned int j = 0; j < components; ++j )
        {
            Comser::ComponentType componentType;
            in.readvar( componentType );

            // Find the component type
            const auto& info = *findInfo( componentType );

            // Construct the component in-place. Wooooooooo that means no templating on component needed here
            // Stupid bind needing reference wrappers though.
            auto placementConstructor = std::bind( info.loader, std::ref( in ), std::placeholders::_1 );
            _group.addComponent( entityId, info.localType, info.size, placementConstructor );
        }
    }
}

void    GroupPersistence::save( std::ostream& out )
{
    // Keep a place here to write the number of grid items
    size_t entities = (size_t)std::distance( _group.begin(), _group.end() );
    out.writevar( entities );

    // Now write our entities
    for( auto entity = _group.begin(); entity != _group.end(); ++entity )
    {
        auto componentsCountPos = out.tellp();
        unsigned int components = 0;
        out.writevar( components );

        // If the entity is one of the ones we want to save, then do so
        for( auto comp = entity->begin(); comp != entity->end(); ++comp )
            if( auto info = findInfo( _group[comp->type] ) )
            {
                out.writevar( info->type );

                info->saver( out, *std::next( _group.begin( comp->type ), comp->index ) );

                components++;
            }

        // Write our component count then return to end
        auto streamPos = out.tellp();
        out.seekp( componentsCountPos );
        out.writevar( components );
        out.seekp( streamPos );
    }
}