#pragma once

#include "Scene.h"

#include "GridPosition.h"

namespace Comser
{
    namespace Grid
    {
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

            typedef std::vector<ComponentDef>   Entity;
            typedef std::vector<Entity>         Tile;

            typedef sigc::signal<void, const Position&, const Position&>    SignalPositionChange;

            struct Handle
            {
                Handle( SignalPositionChange& signal )
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
        public:
            Scene( const std::initializer_list<ComponentType>& types, unsigned int width, unsigned int height )
                : Comser::Scene( types ), _width( width ), _height( height )
            {
                _tiles = new Tile[width * height];
            }
            ~Scene()
            {
                delete[] _tiles;
            }

            EntityHandle        createEntity( const Position& pos );
            void                destroyEntity( EntityHandle& handle );
            void                destroyEntity( const Position& pos );

            Position            entityPos( EntityHandle& handle ) const
            {
                return getPos( handle );
            }

            void                moveEntity( const Position& a, const Position& b );

            Component*          getComponent( EntityHandle& handle, ComponentType type );


            sigc::connection    connectPositionChange( SignalPositionChange::slot_type slot )
            {
                return _positionChange.connect( slot );
            }

            SceneType           type()
            {
                return SceneType::TILE_SET;
            }
        private:
            Position&           getPos( EntityHandle& handle ) const
            {
                return reinterpret_cast<Handle*>(handle.get())->pos;
            }

            unsigned int            _width;
            unsigned int            _height;

            SignalPositionChange    _positionChange;

            Tile*                   _tiles;
        };
    }
}