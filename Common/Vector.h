#pragma once
#ifndef VECTOR_H
#define VECTOR_H

#include <math.h>

namespace Maths
{
    // TODO: template?
    struct Vector
    {
        Vector()
            : _x( 0.0f ), _y( 0.0f ), _z( 0.0f )
        {
        }
        Vector( float x, float y, float z )
            : _x( x ), _y( y ), _z( z )
        {
        }
        Vector( Vector&& x )
            : _x( x._x ), _y( x._y ), _z( x._z )
        {
            x._x = 0;
            x._y = 0;
            x._z = 0;
        }
        Vector( const Vector& x )
            : _x( x._x ), _y( x._y ), _z( x._z )
        {
        }

        float   x() const
        {
            return _x;
        }
        float   y() const
        {
            return _y;
        }
        float   z() const
        {
            return _z;
        }

        void    x( float a )
        {
            _x = a;
        }
        void    y( float a )
        {
            _y = a;
        }
        void    z( float a )
        {
            _z = a;
        }

        // Multiplies each member by its counterpart
        Vector      operator * ( const Vector& v )
        {
            return Vector( _x * v._x, _y * v._y, _z * v._z );
        }
        Vector      operator / ( const Vector& v );
        Vector      operator + ( const Vector& v );
        Vector      operator - ( const Vector& v );
        Vector&     operator *=( const Vector& v );
        Vector&     operator /=( const Vector& v );
        Vector&     operator +=( const Vector& v )
        {
            _x += v._x;
            _y += v._y;
            _z += v._z;

            return *this;
        }
        Vector&     operator -=( const Vector& v );

        Vector      operator * ( float d )
        {
            return Vector( _x * d, _y * d, _z * d );
        }
        Vector      operator / ( float d )
        {
            return Vector( _x / d, _y / d, _z / d );
        }
        Vector      operator + ( float d );
        Vector      operator - ( float d );
        Vector&     operator *=( float d );
        Vector&     operator /=( float d );
        Vector&     operator +=( float d );
        Vector&     operator -=( float d );

        void        invert();

        float       magnitude()
        {
            return (float)sqrt( _x*_x + _y*_y + _z*_z );
        }

        // Gives this vector with magnitude of 1
        Vector      unit()
        {
            return (*this)/magnitude();
        }

        Vector&     cross( const Vector& v );
        float       dot( const Vector& v );
    private:

        float _x,_y,_z;
    };

    struct Vector2D
    {
        Vector2D()
            : _x( 0.0f ), _y( 0.0f )
        {
        }
        Vector2D( float x, float y )
            : _x( x ), _y( y )
        {
        }

        float x() const
        {
            return _x;
        }
        float y() const
        {
            return _y;
        }

    private:
        float _x;
        float _y;
    };
}

#endif