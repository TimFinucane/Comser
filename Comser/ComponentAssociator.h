#pragma once

#include <vector>

#include "Component.h"

namespace Comser
{
    /// <summary>
    /// An explicitly typed integer used to reference
    ///  a certain component type for a single group.
    /// </summary>
    struct    LocalComponentType
    {
        friend struct ComponentAssociator;
    public:
        LocalComponentType()
            : _type( 0 )
        {
        }

        int get() const
        {
            return _type;
        }

        bool    operator > ( const LocalComponentType b ) const
        {
            return _type > b._type;
        }
        bool    operator < ( const LocalComponentType b ) const
        {
            return _type < b._type;
        }
        bool    operator ==( const LocalComponentType b ) const
        {
            return _type == b._type;
        }
        bool    operator !=( const LocalComponentType b ) const
        {
            return _type != b._type;
        }
    private:
        LocalComponentType( int type )
            : _type( type )
        {
        }
        /// <summary>
        /// Sets the LocalComponentType to the type.
        /// Is private so only the Associator can do this
        /// </summary>
        void operator =( int type )
        {
            _type = type;
        }

        int _type;
    };

    /// <summary>
    /// Class that creates the association with a component's global ComponentType,
    ///  and the LocalComponentType for that component in one Group.
    /// </summary>
    struct ComponentAssociator
    {
    public:
        /* TODO
        * Give it the ComponentTypes, it forms the LocalComponentTypes
        */
        ComponentAssociator( const std::initializer_list<ComponentType>& types );
        ~ComponentAssociator()
        {
            delete[] _types;
            _count = 0;
        }

        static LocalComponentType   NOT_FOUND()
        {
            return LocalComponentType( -1 );
        }
        static bool                 NOT_FOUND( LocalComponentType type )
        {
            return type.get() == -1;
        }

        /* TODO
        * Put in the global ComponentType, it gets the LocalComponentType
        */
        LocalComponentType  operator[]( ComponentType type ) const;
        ComponentType       operator[]( LocalComponentType ltype ) const
        {
            return _types[ltype.get()];
        }
    private:
        size_t          _count;
        ComponentType*  _types;
    };
}