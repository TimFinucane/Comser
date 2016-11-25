#pragma once
#pragma once

#include <memory>
#include <algorithm>
#include "StrongEntityCache.h"
#include "Scene.h"

namespace Comser
{
    // TODO: Memory efficiency for loading into cpu?
    /// <summary>
    /// A position in the grid.
    /// Can be used as a unique key to an entity (as long as it doesn't move).
    /// </summary>
    struct MultiGridPosition
    {
        unsigned int x;
        unsigned int y;
        unsigned int z;

        bool operator ==( const MultiGridPosition& pos ) const
        {
            return (x == pos.x && y == pos.y && z == pos.z);
        }
        bool operator !=( const MultiGridPosition& pos ) const
        {
            return !(*this == pos);
        }
        bool operator <( const MultiGridPosition& pos ) const
        {
            return x < pos.x || (x == pos.x && (y < pos.y || (y == pos.y && z < pos.z)));
        }
    };

    // Naive-ish implementation. Will fix at some point if it's necessary.
    // TODO: Strong handle cache using container of weak ptrs(custom deleter?)
    // TODO: Docs
    class MultiGrid final : public Scene<MultiGridPosition>
    {
    public:
        typedef MultiGridPosition Position;

        typedef sigc::signal<void, const Position&, const Position&>    SignalPositionChange;

        /// <summary>
        /// Holds info about a single component in an entity.
        /// </summary>
        struct ComponentDef
        {
            ~ComponentDef()
            {
                delete component;
            }

            template<typename COMPONENT, typename... ARGS>
            static ComponentDef create( LocalComponentType type, ARGS... args )
            {
                return ComponentDef( type, (Component*)new COMPONENT( std::forward<ARGS>( args)... ) );
            }

            ComponentDef( ComponentDef&& def )
                : type( def.type ), component( def.component )
            {
                def.component = nullptr;
            }
            ComponentDef( const ComponentDef& def ) = delete;
            ComponentDef& operator =( const ComponentDef& ) = delete;

            LocalComponentType      type;
            Component*              component;

        private:
            ComponentDef( LocalComponentType t, Component* comp )
                : type( t ), component( comp )
            {
            }
        };

    private:
        typedef Position WeakEnt;
        typedef std::vector<ComponentDef>   Entity;

        // TODO: Other signals
        /// <summary>
        /// The entity handle for a specific entity in a multigrid.
        /// Should only be created by the scene
        /// </summary>
        struct Strong
        {
        public:
            Strong( Position pos, MultiGrid* scene )
                : _pos( pos ), _scene( scene )
            {
            }
            ~Strong()
            {
                _scene->_strongCache.remove( _pos );
            }

            Position&   getPosition()
            {
                return _pos;
            }
            Entity&     getEntity()
            {
                return _scene->getEnt( _pos );
            }

            bool operator ==( const Position& pos )
            {
                return _pos == pos;
            }
            bool operator <( const Position& pos )
            {
                return _pos < pos;
            }  
        protected:
            // TODO: Better version
            void positionChange( const Position& from, const Position& to )
            {
                if( from == _pos )
                    _pos = to;
            }
        private:
            MultiGrid*          _scene;
            WeakEnt             _pos;
        };

        typedef StrongEntityCache<Strong>   StrongCache;
    public:
        MultiGrid( const std::initializer_list<ComponentType>& types, unsigned int width, unsigned int height, unsigned int depth )
            : Scene( types ), _width( width ), _height( height ), _depth( depth )
        {
            _tiles = new Entity[width * height * depth];
        }
        ~MultiGrid()
        {
            delete[] _tiles;
        }
        
        void                destroyEntity( const Position& pos );
        
        template<class COMPONENT, typename... ARGS>
        void                addComponent( const Position& pos, ARGS... args )
        {
            LocalComponentType type = localType( COMPONENT::id() );

            getEnt( pos ).emplace_back( ComponentDef::create<COMPONENT, ARGS...>( type, std::forward<ARGS>( args )... ) );
        }
        void                removeComponent( const Position& pos, LocalComponentType type );

        bool                entityExists( const Position& pos )
        {
            return !getEnt( pos ).empty();
        }

        StrongCache::Handle createHandle( const Position& pos )
        {
            return _strongCache.create( pos, this );
        }
        
        void                moveEntity( const Position& a, const Position& b );

        template <typename COMPONENT>
        COMPONENT*          getComponent( const Position& pos )
        {
            return (COMPONENT*)(getComponent( pos, localType( COMPONENT::id() ) ));
        }
        Component*          getComponent( const Position& pos, LocalComponentType type )
        {
            Entity& entity = getEnt( pos );

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
        Entity&                 getEnt( const Position& pos )
        {
            return _tiles[(pos.y * _width + pos.x) * _depth + pos.z];
        }

        StrongCache                 _strongCache;

        unsigned int                _width;
        unsigned int                _height;
        unsigned int                _depth;

        SignalPositionChange        _positionChange;
            
        Entity*                     _tiles;
    };
}