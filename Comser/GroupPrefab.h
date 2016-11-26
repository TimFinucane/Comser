#pragma once
#ifndef PREFAB_H
#define PREFAB_H

#include <tuple>

#include "Component.h"
#include "Group.h"

namespace Comser
{
    // TODO: Work on changes
    // A static type class used for easily sharing certain entity formats (made up of a bunch of components)
    template <class... COMPONENTS>
    struct GroupPrefab
    {
        /// <summary>
        /// Creates the Entity with those components
        /// </summary>
        /// <param name="group">The group that you add the entity to</param>
        /// <param name="args">The arguments for each component you add</param>
        /// <returns>The entity</returns>
        static Group::EntityId create( Group* group, COMPONENTS&&... args )
        {
           Group::EntityId id = group->createEntity();

            addComponents<COMPONENTS...>( id, group, std::forward<COMPONENTS&&>( args )... );

            return id;
        }

    private:
        template<typename COMPONENT, typename ...COMPS>
        static void addComponents( Group::EntityId id, Group* group, COMPONENT&& component, COMPS&&... args )
        {
            group->addComponent<COMPONENT>( id, component );
            addComponents( id, group, std::forward<COMPS&&>( args )... );
        }
        template<typename COMPONENT>
        static void addComponents( Group::EntityId id, Group* group, COMPONENT&& component )
        {
            group->addComponent<COMPONENT>( id, component );
        }
    };
}

#endif