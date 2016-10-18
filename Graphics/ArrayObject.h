#pragma once

namespace Graphics
{
    // An array object is a single buffer used by the
    //  graphics device to render objects
    template <typename OBJECT>
    class ObjectArray
    {
    public:
        ObjectArray();
        ~ObjectArray();

        /// <summary>
        /// Pulls the array over to memory so that it can
        ///  be modified by the program
        /// </summary>
        void    pull();

        /// <summary>
        /// Updates the graphics device with any changes that
        ///  the program has made
        /// </summary>
        void    push();

        /// <summary>
        /// Adds the given object to the array.
        /// Only works when object has been pulled
        /// </summary>
        /// <returns>Index of added object</returns>
        unsigned int    add( const OBJECT& object );
        unsigned int    add( OBJECT&& object ); // For move semantics

        void            remove( unsigned int object );
        
    };
}