#pragma once
#pragma once

#include <algorithm>
#include "Scene.h"

namespace Comser
{
    // Naive-ish implementation. Will fix at some point if it's necessary.
    // TODO: Strong handle cache using container of weak ptrs(custom deleter?)
    // TODO: Docs
    class MultiGrid final : public Comser::Scene
    {
    public:
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

    private:
        typedef Position WeakEnt;
        typedef std::vector<ComponentDef>   Entity;

        // TODO: Other signals
        /// <summary>
        /// The entity handle for a specific entity in a multigrid.
        /// Should only be created by the scene
        /// </summary>
        struct Strong : StrongEntity
        {
        public:
            Strong( Position pos, MultiGrid* scene )
                : _pos( pos )
            {
                _conPosChange = scene->connectPositionChange( sigc::mem_fun( this, &Strong::positionChange ) );
            }
            ~Strong()
            {
                _conPosChange.disconnect();
            }

            WeakPtr getEntity()
            {
                return &_pos;
            }

        protected:
            // TODO: Better version
            void positionChange( const Position& from, const Position& to )
            {
                if( from == _pos )
                    _pos = to;
            }
        private:
            WeakEnt             _pos;
                
            sigc::connection    _conPosChange;
        };
    public:
        MultiGrid( const std::initializer_list<ComponentType>& types, unsigned int width, unsigned int height, unsigned int depth )
            : Comser::Scene( types ), _width( width ), _height( height )
        {
            _tiles = new Entity[width * height * depth];
        }
        ~MultiGrid()
        {
            delete[] _tiles;
        }
            
        WeakHandle          createEntity( const Position& pos );
        void                destroyEntity( WeakPtr ent );
        void                destroyEntity( const Position& pos );
        

        static WeakPtr      ptrFromPos( Position& pos )
        {
            return &pos;
        }
        bool                entityExists( WeakPtr ent );

        StrongHandle        makeStrong( WeakHandle weakEnt );

        static Position&    getPos( WeakPtr ent )
        {
            return *reinterpret_cast<Position*>(ent);
        }
            
        void                moveEntity( const Position& a, const Position& b );
        Component*          getComponent( WeakPtr ent, LocalComponentType type )
        {
            Entity& entity = getEnt( ent );

            return std::find_if( entity.begin(), entity.end(), [type]( const ComponentDef& comp ){ return comp.type == type; } )->component;
        }
        sigc::connection    connectPositionChange( SignalPositionChange::slot_type slot )
        {
            return _positionChange.connect( slot );
        }
        
        unsigned int        width() const
        {
            return _width;
        }
        unsigned int        height() const
        {
            return _height;
        }
        unsigned int        depth() const
        {
            return _depth;
        }

        SceneType           type() const
        {
            return SceneType::MULTIGRID;
        }
    private:
        Entity&             getEnt( const Position& pos )
        {
            return _tiles[(pos.y * _width + pos.x) * _depth + pos.z];
        }
        Entity&             getEnt( const WeakPtr handle )
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