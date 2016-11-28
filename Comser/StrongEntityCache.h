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

        struct Allocator
        {
        public:
            typedef STRONG value_type;

            // Allow conversion to the basic std::allocator when we arent operating on the STRONG type
            template <typename U>
            struct rebind
            {
                typedef std::allocator<U> other;
            };
            template <typename T>
            operator std::allocator<T>() const
            {
                return std::allocator<T>();
            }
            
            Allocator( StrongEntityCache& cache )
                : _cache( cache )
            {
            }

            STRONG* allocate( std::size_t n )
            {
                return ::operator new[](n * sizeof( STRONG ));
            }
            void    deallocate( STRONG* strong, std::size_t n )
            {
                ::operator delete[](strong, n * sizeof( STRONG ));
                _cache->remove( *strong );
            }
        private:
            StrongEntityCache& _cache;
        };
    public:
        StrongEntityCache()
            : _allocator( *this )
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
                locked = std::allocate_shared<STRONG>( _allocator, item, args... );
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
        Allocator   _allocator;
        Cache       _cache;
    };
}