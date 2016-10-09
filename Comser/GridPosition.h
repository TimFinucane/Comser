#pragma once

namespace Comser
{
    namespace Grid
    {
        // TODO: Memory efficiency for loading into cpu?
        /// <summary>
        /// A position in the grid.
        /// Can be used as a unique key to an entity (as long as it doesn't move).
        /// </summary>
        struct Position
        {
            unsigned int x;
            unsigned int y;
            unsigned int z;

            bool operator ==( const Position& pos ) const
            {
                return (x == pos.x && y == pos.y && z == pos.z);
            }
        };
    }
}