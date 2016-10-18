#pragma once 
#ifndef MATRIX_H
#define MATRIX_H

#include <initializer_list>

#include "Vector.h"

namespace Maths
{
    // This uses row-major convention
    template <int ROWS, class CLASS>
    class BaseMatrix
    {
    public:
        static const unsigned int COLUMNS = ROWS;
        static const unsigned int SIZE = (ROWS * COLUMNS);

        BaseMatrix( std::initializer_list<float> list )
        {
            int i = 0;
            for( auto j = list.begin(); j != list.end(); ++i, ++j )
                _d[i] = *j;
        }
        BaseMatrix( float d[SIZE] )
        {
            for( unsigned int i = 0; i < SIZE; ++i )
                _d[i] = d[i];
        }
        BaseMatrix()
        {
            for( int i = 0; i < SIZE; ++i )
                _d[i] = 0.0f;
        }

        CLASS   operator + ( const CLASS& m );
        CLASS   operator - ( const CLASS& m );
        CLASS   operator * ( const CLASS& m );
        CLASS   operator / ( const CLASS& m );

        CLASS&  operator +=( const CLASS& m );
        CLASS&  operator -=( const CLASS& m );
        CLASS&  operator *=( const CLASS& m );
        CLASS&  operator /=( const CLASS& m );

        static CLASS   identity()
        {
            CLASS mat;

            for( int i = 0; i < ROWS; ++i )
            {
                mat( i, i ) = 0.0f;
            }

            return mat;
        }
        
        // TODO: Reflections

        float&          operator()( unsigned int index )
        {
            return _d[index];
        }
        float&          operator()( unsigned int row, unsigned int column )
        {
            return _d[(row*COLUMNS) + column];
        }
        const float     operator()( unsigned int row, unsigned int column ) const
        {
            return _d[(row*COLUMNS) + column];
        }

        float&          operator[]( unsigned int index )
        {
            return _d[index];
        }
        const float&    operator[]( unsigned int index ) const
        {
            return _d[index];
        }

        static CLASS   scale( float scale[ROWS] )
        {
            CLASS mat;
            for( int i = 0; i < ROWS; ++i )
            {
                mat( i, i ) = scale[i];
            }

            return mat;
        }
    private:
        float _d[SIZE];
    };

    template <int ROWS>
    class Matrix : public BaseMatrix<ROWS, Matrix<ROWS>>
    {};

    template<>
    class Matrix<2> : public BaseMatrix<2, Matrix<2>>
    {
        // TODO: Specialisation here
    };

    // For 3D operations, along with a 4th column for translations
    template <>
    class Matrix<4> : public BaseMatrix<4, Matrix<4>>
    {
    public:
        // Static creators
        // All rotation units are in radians, not degrees
        // Rotates in a counterclockwise manner
        static Matrix<4>    rotate( float x, float y, float z );
        static Matrix<4>    rotate( float amt, Vector3D axis );
        static Matrix<4>    rotateX( float angle );
        static Matrix<4>    rotateY( float angle );
        static Matrix<4>    rotateZ( float angle );
    };

    typedef Matrix<4> Matrix4;
    typedef Matrix<2> Matrix2;
}

#endif