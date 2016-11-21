#pragma once
#pragma once

#include <algorithm>
#include "Scene.h"

namespace Comser
{
    // Naive-ish implementation. Will fix at some point if it's necessary.
    // TODO: Strong handle cache using container of weak ptrs(custom deleter?)
    // TODO: Docs
    class MultiGrid final : public Scene
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
            bool operator !=( const Position& pos ) const
            {
                return !(*this == pos);
            }
            bool operator <( const Position& pos ) const
            {
                return x < pos.x || (x == pos.x && (y < pos.y || (y == pos.y && z < pos.z)));
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
                : _pos( pos ), _scene( scene )
            {
            }
            ~Strong()
            {
                _scene->strongHandleDeleter( this );
            }

            WeakPtr     getWeak()
            {
                return &_pos;
            }
            Position&   getPosition()
            {
                return _pos;
            }
            Entity&     getEntity()
            {
                return _scene->getEnt( _pos );
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

        typedef std::vector<StrongHandle>   StrongCache;
    public:
        MultiGrid( const std::initializer_list<ComponentType>& types, unsigned int width, unsigned int height, unsigned int depth )
            : Scene( types ), _width( width ), _height( height )
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
        bool                entityExists( WeakPtr ent )
        {
            return !getEnt( ent ).empty();
        }

        StrongHandle        makeStrong( WeakHandle weakEnt )
        {
            auto it = getCacheIterator( *(Position*)weakEnt.get() );
            if( getStrong( (*it).get() )->getPosition() == getPos( weakEnt.get() ) )
                it = _strongCache.insert( it, std::make_shared<Strong>( Strong( getPos( weakEnt.get() ), this ) ) );

            return *it;
        }

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
        Entity&                 getEnt( const Position& pos )
        {
            return _tiles[(pos.y * _width + pos.x) * _depth + pos.z];
        }
        Entity&                 getEnt( const WeakPtr handle )
        {
            return getEnt( getPos( handle ) );
        }
        static Strong*          getStrong( const StrongPtr ptr )
        {
            return reinterpret_cast<Strong*>(ptr);
        }

        StrongCache::iterator   getCacheIterator( StrongPtr ptr )
        {
            return std::lower_bound( _strongCache.begin(), _strongCache.end(), getStrong( ptr )->getPosition(),
                []( const StrongHandle& handle, const Position& position )
            {
                return getStrong( handle.get() )->getPosition() < position;
            } );
        }
        StrongCache::iterator   getCacheIterator( const Position& position )
        {
            return std::lower_bound( _strongCache.begin(), _strongCache.end(), position, 
                []( const StrongHandle& handle, const Position& position )
                {
                    return getStrong( handle.get() )->getPosition() < position;
                } );
        }
        void                    strongHandleDeleter( Strong* strong )
        {
            auto it = getCacheIterator( strong );
            if( &**it == strong )
                _strongCache.erase( it );
        }

        std::vector<StrongHandle>   _strongCache;

        unsigned int                _width;
        unsigned int                _height;
        unsigned int                _depth;

        SignalPositionChange        _positionChange;
            
        Entity*                     _tiles;
    };
}