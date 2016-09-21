#pragma once
#ifndef RESOURCE_HANDLER_H
#define RESOURCE_HANDLER_H

#include <string>
#include <vector>


namespace Comser
{
    namespace Resource
    {
        /*
         * <summary>
         * A class that holds and handles all resources of a given type
         * </summary>
         * <typeparam name="RESOURCE">The class being handled</typeparam>
         */
        template< class RESOURCE >
        class Handler
        {
        public:
            typedef unsigned __int32    Index;
        public:
            Index       create( std::string file );
            void        destroy( Index index );

            RESOURCE*   operator()( Index index );
        protected:
            /*
             * <summary>
             * Is defined at a later stage, when Handler<RESOURCE> is declared.
             * Allocates and creates the resource.
             * </summary>
             * <param name="data">The file</param>
             * <param name="size">The file size</param>
             */
            // TODO: Pass in allocator
            static RESOURCE*    createResource( const char* data, size_t size );
            /*
             * <summary>
             * Is defined at a later stage, when Handler<RESOURCE> is declared.
             * Destroys and deallocates the resource.
             * </summary>
             * <param name="resource">The resource to destroy</param>
             */
            static void         destroyResource( RESOURCE* resource );
            /*
             * <summary>
             * Is defined at a later stage, when Handler<RESOURCE> is declared.
             * Creates a new instance of the resource.
             * </summary>
             * <param name="resource">The original resource</param>
             */
            // TODO: Pass in allocator
            static RESOURCE*    copyResource( const RESOURCE* resource );

        private:
            struct Instance
            {
                unsigned __int32    users; // How many references there are to the object
                RESOURCE*           resource;
            };
            std::vector<Instance>   _instances;
        };


    }
}

#endif