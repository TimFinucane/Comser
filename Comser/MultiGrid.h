#pragma once
#pragma once

#include "Scene.h"

#include "GridPosition.h"

namespace Comser
{
    namespace MultiGrid
    {
        // TODO: Memory efficiency for loading into cpu?
        /// <summary>
        /// A position in the grid.
        /// Can be used as a unique key to an entity (as long as it doesn't move).
        /// </summary>
        struct Position
        {
            unsigned int x;
            unsigned int y;
            unsigned int z;

            bool operator ==( const Position& pos ) const
            {
                return (x == pos.x && y == pos.y && z == pos.z);
            }
        };

        typedef sigc::signal<void, const Position&, const Position&>    SignalPositionChange;

        // TODO: Naive-ish implementation. Will fix at some point if it's necessary.
        // TODO: Docs
        class Scene final : public Comser::Scene
        {
        public:
            /// <summary>
            /// Holds info about a single component in an entity.
            /// </summary>
            struct ComponentDef
            {
                template<typename COMPONENT, typename... ARGS>
                ComponentDef( LocalComponentType t, ARGS... args )
                {
                    type = t;
                    component = (Component*)(new COMPONENT( args ));
                }
                ~ComponentDef()
                {
                    delete component;
                }

                LocalComponentType      type;
                Component*              component;
            };

            /// <summary>
            /// The entity handle for a specific entity in a multigrid.
            /// Should only be created by the scene
            /// </summary>
            struct Handle
            {
            public:
                Handle( Position p, SignalPositionChange& signal )
                    : pos( p )
                {
                    _con = signal.connect( sigc::mem_fun( this, &Handle::positionChange ) );
                }
                ~Handle()
                {
                    _con.disconnect();
                }

                Position pos;

            protected:
                void positionChange( const Position& from, const Position& to )
                {
                    if( from == pos )
                        pos = to;
                }
            private:
                sigc::connection _con;
            };

            typedef std::vector<ComponentDef>   Entity;
        public:
            Scene( const std::initializer_list<ComponentType>& types, unsigned int width, unsigned int height, unsigned int depth )
                : Comser::Scene( types ), _width( width ), _height( height )
            {
                _tiles = new Entity[width * height * depth];
            }
            ~Scene()
            {
                delete[] _tiles;
            }

            EntityHandle        createEntity( const Position& pos );
            void                destroyEntity( EntityHandle& handle );
            void                destroyEntity( const Position& pos );

            const Position&     getPos( const EntityHandle& handle ) const
            {
                return reinterpret_cast<Handle*>(handle.get())->pos;
            }
            Position&           getPos( EntityHandle& handle ) const
            {
                return reinterpret_cast<Handle*>(handle.get())->pos;
            }

            void                moveEntity( const Position& a, const Position& b );

            Component*          getComponent( EntityHandle& handle, ComponentType type );


            sigc::connection    connectPositionChange( SignalPositionChange::slot_type slot )
            {
                return _positionChange.connect( slot );
            }

            SceneType           type()
            {
                return SceneType::MULTIGRID;
            }
        private:

            Entity&             getEnt( const Position& pos )
            {
                return _tiles[(pos.y * _width + pos.x) * _depth + pos.z];
            }
            Entity&             getEnt( const EntityHandle& handle )
            {
                return getEnt( getPos( handle ) );
            }

            unsigned int            _width;
            unsigned int            _height;
            unsigned int            _depth;

            SignalPositionChange    _positionChange;

            Entity*                 _tiles;
        };
    }
}