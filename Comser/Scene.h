#pragma once

#include <initializer_list>

#include <sigc++/sigc++.h>

#include "Component.h"
#include "ComponentAssociator.h"

namespace Comser
{
    enum class SceneType
    {
        NONE,
        GROUP,
        GRID,
        MULTIGRID
    };

    // TODO: Comments

    class SceneBase abstract
    {
        friend class Engine;

    public:
        SceneBase( const std::initializer_list<ComponentType>& types )
            : _associator( types )
        {
        }
        virtual ~SceneBase() = default;
        
        ComponentType           operator[]( LocalComponentType ltype )
        {
            return _associator[ltype];
        }
        LocalComponentType      operator[]( ComponentType type )
        {
            return _associator[type];
        }
        LocalComponentType      localType( ComponentType type )
        {
            return (*this)[type];
        }

        bool                    active() const
        {
            return _active;
        }

        virtual SceneType       type() const
        {
            return SceneType::NONE;
        }

    protected:
        virtual void    onEnable() {};
        virtual void    onDisable() {};

        void            disable()
        {
            _active = false;
            onDisable();
        }
        void            enable()
        {
            _active = true;
            onEnable();
        }

    private:
        bool                _active;
        ComponentAssociator _associator;
    };

    template <typename ENTITYREF>
    class Scene abstract : public SceneBase
    {
    public:
        using CONSTREF = typename std::conditional<std::is_fundamental<ENTITYREF>::value, ENTITYREF, const ENTITYREF&>::type;

        using Signal = typename sigc::signal<void, CONSTREF, Component*>;
        using SignalList = typename std::vector<Signal>;

    public:
        Scene( const std::initializer_list<ComponentType>& types )
            : SceneBase( types )
        {
            _addedSignals.resize( types.size() );
            _removedSignals.resize( types.size() );
        }
        virtual ~Scene() = default;
       
        template <typename COMPONENT>
        sigc::connection        connectAdded( sigc::slot<void, CONSTREF, COMPONENT*> slot )
        {
            static_assert(std::is_base_of<Component, COMPONENT>::value, "Slot must be accepting a component");
            LocalComponentType type = localType( COMPONENT::id() );

            return _addedSignals[type.get()].connect( sigc::retype( slot ) );
        }
        sigc::connection        connectAdded( typename Signal::slot_type slot, LocalComponentType type )
        {
            return _addedSignals[type.get()].connect( slot );
        }
        template <typename COMPONENT>
        sigc::connection        connectRemoved( sigc::slot<void, CONSTREF, COMPONENT*> slot )
        {
            static_assert(std::is_base_of<Component, COMPONENT>::value, "Slot must be accepting a component");
            LocalComponentType type = localType( COMPONENT::id() );

            return _removedSignals[type.get()].connect( sigc::retype( slot ) );
        }
        sigc::connection        connectRemoved( typename Signal::slot_type slot, LocalComponentType type )
        {
            return _removedSignals[type.get()].connect( slot );
        }

        virtual Component*      getComponent( CONSTREF id, LocalComponentType localType ) = 0;

    protected:
        void            signalAdded( LocalComponentType comType, CONSTREF entPtr, Component* component )
        {
            _addedSignals[comType.get()].emit( entPtr, component );
        }
        void            signalRemoved( LocalComponentType comType, CONSTREF entPtr, Component* component )
        {
            _removedSignals[comType.get()].emit( entPtr, component );
        }

    private:
        SignalList      _addedSignals;
        SignalList      _removedSignals;
    };
}