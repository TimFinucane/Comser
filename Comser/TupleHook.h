#pragma once
#ifndef TUPLE_HOOK_H
#define TUPLE_HOOK_H

#include <tuple>
#include <map>
#include <array>

#include "Component.h"
#include "Entity.h"

#include "Group.h"

#include "Signal.h"


namespace Comser
{
    namespace Scene
    {
        template<class COMPONENT>
        LocalComponentType _idHelper()
        {
            return COMPONENT::id();
        }

        // Purpose: Used by a system to keep a reference to all
        //  entities which contain the desired components (tuple).
        // For sake of efficiency, put components in order of least->most used.
        template<class... COMPONENTS>
        class TupleHook //: public sigc::trackable
        {
        public:
            typedef std::tuple<COMPONENTS*...>                              Tuple;

            typedef std::map<EntityId,Tuple>                                Map;
            typedef typename Map::iterator                                  Iterator;
            typedef typename Map::const_iterator                            ConstIterator;

            //typedef std::pair<EntityId,Tuple>                               Pair;

            typedef Event::Signal::Signal1<typename Map::value_type*>                Signal;

        public:
            // TODO: Find all entities already created with those props
            TupleHook( Group* group )
                : _localTypes{ (*group)[COMPONENTS::id()]... }, _group( group )
            {
                for( int i = 0; i < _localTypes.size(); ++i )
                {
                    group->connectAdded( _localTypes[i], sigc::mem_fun1( *this, &TupleHook<COMPONENTS...>::_componentAdded )  );
                    group->connectRemoved( _localTypes[i], sigc::mem_fun1( *this, &TupleHook<COMPONENTS...>::_componentRemoved )  );
                }
            }

            ConstIterator       begin() const
            {
                return _tuples.begin();
            }
            ConstIterator       end() const
            {
                return _tuples.end();
            }

            unsigned int        size()
            {
                return _tuples.size();
            }

            Signal              tupleAdded;
            Signal              tupleRemoved;
        private:
            static const unsigned __int32  COMPONENT_COUNT  = sizeof...(COMPONENTS);

            typedef std::array<LocalComponentType,COMPONENT_COUNT>  LocalTypes;
        private:
            // Purpose: To add to the map components which were created before this TupleHook
            void                _addCreatedComponents()
            {
                for( ComponentVector::ConstIterator i = _group->begin( _localTypes[0] ); i != _group->end( _localtypes[0] ); ++i )
                {
                    // Now check if each of these components has all other components
                    Tuple tuple;
                    // Running recurse check for all but the first component
                    if( _recurseCheck( _group, tuple, &types[1] ) )
                    {
                        // Put in that first component
                        std::get<0>(tuple) = reinterpret_cast<COMPONENTS>( i->component );

                        // Add it to our map
                        _tuples.emplace( i->entity, tuple );
                        tupleAdded( _tuples.back() );
                    }
                }
            }

            void                _componentAdded( ComponentVector::ComponentInfo* componentInfo )
            {
                EntityId ent = componentInfo->entity;
                Tuple tuple;

                // Check to see if all the types are there
                if( _recurseCheck( _group, ent, tuple, _localTypes ) )
                {
                    _tuples.emplace( ent, tuple );
                    tupleAdded( &(*_tuples.rbegin()) );
                }
            }
            void                _componentRemoved( ComponentVector::ComponentInfo* componentInfo )
            {
                auto it = _tuples.find( componentInfo->entity );
                if( it != _tuples.end() )
                {
                    tupleRemoved( &(*it) );
                    _tuples.erase( it );
                }
            }

            // If true, all components are present
            template <int STEPS = 0>
            bool         _recurseCheck( Group* group, EntityId id, Tuple& tuple, LocalTypes& types ) const
            {
                std::get<STEPS>(tuple) = 
                    reinterpret_cast<std::tuple_element<STEPS,Tuple>::type>( group->getComponent( id, types[STEPS] ) );
                
                return (std::get<STEPS>(tuple) != nullptr) && _recurseCheck<STEPS+1>( group, id, tuple, types );
            }
            template <>
            bool         _recurseCheck<COMPONENT_COUNT>( Group* group, EntityId id, Tuple& tuple, LocalTypes& types ) const
            {
                (group);
                (id);
                (tuple);
                (types);
                return true;
            }

            Group*              _group;
            LocalTypes          _localTypes;

            Map                 _tuples;
        };
    }
}
#endif