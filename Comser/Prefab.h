#pragma once
#ifndef PREFAB_H
#define PREFAB_H

#include "Component.h"
#include "Entity.h"
#include "Group.h"

namespace Comser
{
    // TODO: Work on changes
    // A static type class used for easily sharing certain entity formats (made up of a bunch of components)
    template <class... COMPONENTS>
    class Prefab
    {
        /*
         * <summary>
         * Creates the Entity with those components
         * </summary>
         * <param name="group">The group that you add the entity to</param>
         * <param name="args">The arguments for each component you add</param>
         * <returns>The entity</returns>
         */
        static EntityHandle operator()( Scene* group, COMPONENTS&&... args )
        {
            EntityId entity = group->createEntity();

            group->addComponent<COMPONENTS,COMPONENTS&&>( entity, args )...;

            return entity;
        }

        /*
         * <summary>
         * Destroys the entity.
         * Is the same as calling Group->destroyEntity
         * </summary>
         * <param name="group">The group you made the entity in</param>
         * <param name="id">The entity</param>
         */
        static void         operator()( Scene* group, EntityHandle id )
        {
            group->destroyEntity( id );
        }
    };
}

#endif