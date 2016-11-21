#pragma once
#ifndef ENTITY_H
#define ENTITY_H

#include <memory>
#include <sigc++/sigc++.h>

#include "ComponentAssociator.h"

namespace Comser
{
    /// An opaque handle, used to access the entity
    ///  of the scene that handed the handle to you.
    /// The underlying type is defined by the scene,
    ///  so don't try handing it to any other scene but
    ///  the one you got it from.
    /// Note that these are meant to be temporary. The entity
    ///  may have been deleted without the entity handle knowing.

    typedef void WeakEntity;
    typedef WeakEntity*     WeakPtr;
    typedef std::shared_ptr<WeakEntity>     WeakEntityHandle;


    struct StrongEntity abstract
    {
        typedef class sigc::signal<void, LocalComponentType, Component*> ComponentSignal;
        typedef class sigc::signal<void> DeleteSignal;
    public:
        sigc::connection    onAdded( ComponentSignal::slot_type slot )
        {
            return added.connect( slot );
        }
        sigc::connection    onRemoved( ComponentSignal::slot_type slot )
        {
            return removed.connect( slot );
        }

        sigc::connection    onDestroyed( DeleteSignal::slot_type slot )
        {
            return destroyed.connect( slot );
        }

        virtual WeakPtr     getWeak() = 0;
        //virtual bool isDeleted(); Should do?
    protected:
        void    add( LocalComponentType type, Component* component )
        {
            added.emit( type, component );
        }
        void    remove( LocalComponentType type, Component* component )
        {
            removed.emit( type, component );
        }
        void    destroy()
        {
            destroyed.emit();
        }

    private:
        ComponentSignal added;
        ComponentSignal removed;
        DeleteSignal    destroyed;
    };
    typedef StrongEntity*   StrongPtr;
    typedef std::shared_ptr<StrongEntity>   StrongEntityHandle;

    
    typedef WeakEntityHandle    WeakHandle;
    typedef StrongEntityHandle  StrongHandle;
}

#endif