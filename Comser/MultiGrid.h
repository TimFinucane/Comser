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
            template <typename COMPONENT, typename... ARGS>
            ComponentDef( LocalComponentType type, ARGS... args )
                : type( type ), component( (Component*)new COMPONENT( std::forward<ARGS>( args )... ) )
            {
            }
            ComponentDef( LocalComponentType t, Component* comp )
                : type( t ), component( comp )
            {
            }

            ~ComponentDef()
            {
                delete component;
            }

            ComponentDef( ComponentDef&& def ) noexcept
                : type( def.type ), component( def.component )
            {
                def.component = nullptr;
            }
            ComponentDef& operator =( ComponentDef&& def ) noexcept
            {
                std::swap( type, def.type );
                std::swap( component, def.component );

                return *this;
            }
            ComponentDef( const ComponentDef& def ) = delete;
            ComponentDef& operator =( const ComponentDef& ) = delete;

            LocalComponentType      type;
            Component*              component;

        private:
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
            Strong( Position pos )
                : _pos( pos )
            {
            }
            ~Strong()
            {
            }

            Position&   getPosition()
            {
                return _pos;
            }

            operator WeakEnt&()
            {
                return _pos;
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
            WeakEnt             _pos;
        };

        typedef StrongEntityCache<Strong>   StrongCache;
    public:
        MultiGrid( const std::initializer_list<ComponentType>& types, unsigned int width, unsigned int height, unsigned int depth )
            : Comser::Scene<MultiGridPosition>( types ), _width( width ), _height( height ), _depth( depth )
        {
            _tiles = new Entity[width * height * depth];
        }
        ~MultiGrid()
        {
            delete[] _tiles;
        }
        
        void                destroyEntity( const Position& pos );
        
        template<class COMPONENT, typename... ARGS>
        void                addComponent( const Position& pos, ARGS&&... args )
        {
            LocalComponentType type = localType( COMPONENT::id() );
            Entity& ent = getEnt( pos );

            COMPONENT* com = new COMPONENT( std::forward<ARGS>( args )... );
            ent.emplace_back( type, (Component*)com );

            signalAdded( type, pos, com );
        }

        // An in-place, placement new construction of the object
        template<typename CONSTRUCTOR>
        void                addComponent( const Position& pos, LocalComponentType type, size_t size, const CONSTRUCTOR& constructor )
        {
            void* memory = operator new( size );
            // TODO: Try-catch?
            constructor( memory );

            Entity& ent = getEnt( pos );
            ent.emplace_back( type, (Component*)memory );

            signalAdded( type, pos, com );
        }

        template<class COMPONENT>
        void                removeComponent( const Position& pos )
        {
            removeComponent( pos, localType( COMPONENT::id() ) );
        }
        void                removeComponent( const Position& pos, LocalComponentType type )
        {
            Entity& ent = getEnt( pos );
            auto compIt = std::find_if( ent.begin(), ent.end(), [type]( const ComponentDef& def ) { return def.type == type; } );

            signalRemoved( type, pos, compIt->component );

            ent.erase( compIt );
        }

        bool                entityExists( const Position& pos )
        {
            return !getEnt( pos ).empty();
        }

        StrongCache::Handle createHandle( const Position& pos )
        {
            return _strongCache.create( pos );
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

        const Entity&       getEntity( const Position& pos ) const
        {
            return _tiles[(pos.y * _width + pos.x) * _depth + pos.z];
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