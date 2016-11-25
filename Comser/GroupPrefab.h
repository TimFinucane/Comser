#pragma once
#ifndef PREFAB_H
#define PREFAB_H

#include "Component.h"
#include "Group.h"

namespace Comser
{
    // TODO: Work on changes
    // A static type class used for easily sharing certain entity formats (made up of a bunch of components)
    template <class... COMPONENTS>
    class GroupPrefab
    {
        /// <summary>
        /// Creates the Entity with those components
        /// </summary>
        /// <param name="group">The group that you add the entity to</param>
        /// <param name="args">The arguments for each component you add</param>
        /// <returns>The entity</returns>
        static Group::EntityId operator()( Group* group, COMPONENTS&&... args )
        {
            Group::EntityId entity = group->createEntity();

            group->addComponent<COMPONENTS,COMPONENTS&&>( entity, args )...;

            return entity;
        }
    };
}

#endif