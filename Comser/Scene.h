#pragma once

#include <initializer_list>

#include <sigc++/sigc++.h>

#include "Component.h"
#include "ComponentAssociator.h"

#include "Entity.h"

namespace Comser
{
    enum class SceneType
    {
        NONE,
        GROUP,
        GRID,
        MULTIGRID
    };

    class Scene abstract
    {
        friend class Game;
    public:
        typedef sigc::signal<void, WeakPtr, Component*>         Signal;
        typedef std::vector<Signal>                             SignalList;
    public:
        Scene( const std::initializer_list<ComponentType>& types )
            : _associator( types )
        {
            _addedSignals.resize( types.size() );
            _removedSignals.resize( types.size() );
        }
        virtual ~Scene() = default;

        virtual StrongHandle    makeStrong( WeakHandle entity ) = 0;

        bool                    active() const
        {
            return _active;
        }
        
        sigc::connection        connectAdded( LocalComponentType type, Signal::slot_type slot )
        {
            return _addedSignals[type.get()].connect( slot );
        }
        sigc::connection        connectRemoved( LocalComponentType type, Signal::slot_type slot )
        {
            return _removedSignals[type.get()].connect( slot );
        }

        virtual Component*      getComponent( WeakPtr id, LocalComponentType localType ) = 0;

        LocalComponentType      operator[]( ComponentType type )
        {
            return _associator[type];
        }
        LocalComponentType      localType( ComponentType type )
        {
            return (*this)[type];
        }

        virtual SceneType       type() const
        {
            return SceneType::NONE;
        }
    protected:
        virtual void    onEnable(){};
        virtual void    onDisable(){};

        void            signalAdded( LocalComponentType comType, WeakPtr entPtr, Component* component )
        {
            _addedSignals[comType.get()].emit( entPtr, component );
        }
        void            signalRemoved( LocalComponentType comType, WeakPtr entPtr, Component* component )
        {
            _removedSignals[comType.get()].emit( entPtr, component );
        }

    private:
        SignalList      _addedSignals;
        SignalList      _removedSignals;

        void    disable()
        {
            _active = false;
            onDisable();
        }
        void    enable()
        {
            _active = true;
            onEnable();
        }

        ComponentAssociator     _associator;

        bool                    _active;
    };
}