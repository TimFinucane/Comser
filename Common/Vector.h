#pragma once
#ifndef VECTOR_H
#define VECTOR_H

#include <math.h>

namespace Maths
{
    struct Vector3D
    {
        Vector3D()
            : _x( 0.0f ), _y( 0.0f ), _z( 0.0f )
        {
        }
        Vector3D( float x, float y, float z )
            : _x( x ), _y( y ), _z( z )
        {
        }
        Vector3D( Vector3D&& x )
            : _x( x._x ), _y( x._y ), _z( x._z )
        {
            x._x = 0;
            x._y = 0;
            x._z = 0;
        }
        Vector3D( const Vector3D& x )
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
        Vector3D    operator * ( const Vector3D& v )
        {
            return Vector3D( _x * v._x, _y * v._y, _z * v._z );
        }
        Vector3D    operator / ( const Vector3D& v );
        Vector3D    operator + ( const Vector3D& v );
        Vector3D    operator - ( const Vector3D& v );
        Vector3D&   operator *=( const Vector3D& v );
        Vector3D&   operator /=( const Vector3D& v );
        Vector3D&   operator +=( const Vector3D& v )
        {
            _x += v._x;
            _y += v._y;
            _z += v._z;

            return *this;
        }
        Vector3D&     operator -=( const Vector3D& v );

        Vector3D    operator * ( float d )
        {
            return Vector3D( _x * d, _y * d, _z * d );
        }
        Vector3D    operator / ( float d )
        {
            return Vector3D( _x / d, _y / d, _z / d );
        }
        Vector3D    operator + ( float d );
        Vector3D    operator - ( float d );
        Vector3D&   operator *=( float d );
        Vector3D&   operator /=( float d );
        Vector3D&   operator +=( float d );
        Vector3D&   operator -=( float d );

        void        invert();

        float       magnitude()
        {
            return (float)sqrt( _x*_x + _y*_y + _z*_z );
        }

        // Gives this vector with magnitude of 1
        Vector3D    unit()
        {
            return (*this)/magnitude();
        }

        Vector3D&   cross( const Vector3D& v );
        float       dot( const Vector3D& v );
    private:

        float _x,_y,_z;
    };

    struct Vector2D
    {
        Vector2D()
            : _x( 0.0f ), _y( 0.0f )
        {
        }
        Vector2D( double x, double y )
            : _x( x ), _y( y )
        {
        }

        double x() const
        {
            return _x;
        }
        double y() const
        {
            return _y;
        }

    private:
        double _x;
        double _y;
    };
}

#endif