#include "View.h"

Maths::Matrix4 createOrtho( int left, int right, int top, int bottom )
{
    Maths::Matrix4 mat = Maths::Matrix4::identity();

    mat( 0, 0 ) = 2.0f / (float)(right - left);
    mat( 0, 3 ) = -(right + left) / (float)(right - left);

    mat( 1, 1 ) = 2.0f / (float)(top - bottom);
    mat( 1, 3 ) = -(top + bottom) / (float)(top - bottom);

    mat( 2, 2 ) = -2.0f;
    mat( 2, 3 ) = -1.0f;

    return mat;
}