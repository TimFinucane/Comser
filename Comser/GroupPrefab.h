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
        typedef std::tuple<Group::EntityId, COMPONENTS...> Tuple;

        /// <summary>
        /// Creates the Entity with those components
        /// </summary>
        /// <param name="group">The group that you add the entity to</param>
        /// <param name="args">The arguments for each component you add</param>
        /// <returns>The entity</returns>
        static Tuple create( Group* group, COMPONENTS&&... args )
        {
            Tuple tuple;

            std::get<0>( tuple ) = group->createEntity();

            (std::get<COMPONENTS>( tuple ) = group->addComponent<COMPONENTS,COMPONENTS&&>( entity, args ))...;

            return entity;
        }
    };
}

#endif