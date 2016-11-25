#pragma once

#include <vector>
#include <memory>
#include <algorithm>

namespace Comser
{
    template<typename STRONG>
    struct StrongEntityCache
    {
    public:
        typedef std::shared_ptr<STRONG>                 Handle;
        typedef std::vector<std::weak_ptr<STRONG>>      Cache;
        using Iterator = typename Cache::iterator;
    public:
        StrongEntityCache()
        {
        }

        template <typename T, typename ...ARGS>
        Handle  create( const T& item, ARGS... args )
        {
            Iterator it = getCacheIterator( item );

            Handle locked;
            if( it != _cache.end() && *(locked = it->lock()) == item )
                return locked;
            else
            {
                locked = std::make_shared<STRONG>( item, args... );
                _cache.insert( it, locked );

                return locked;
            }
        }
        template <typename T>
        void    remove( const T& item )
        {
            Iterator it = getCacheIterator( item );
            if( *it->lock().get() == item )
                _cache.erase( it );
        }

    protected:
        template <typename T>
        Iterator    getCacheIterator( const T& t )
        {
            return std::lower_bound( _cache.begin(), _cache.end(), t,
                []( const std::weak_ptr<STRONG>& handle, const T& t )
            {
                return *(handle.lock()) < t;
            } );
        }

    private:
        Cache   _cache;
    };
}