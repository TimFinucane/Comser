#include <CppUnitTest.h>

#include <Matrix.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

TEST_CLASS(MatrixTest)
{
public:
    TEST_METHOD(matrixIdentities)
    {
        Maths::Matrix4 mat = Maths::Matrix4::identity();

        Assert::AreEqual( mat( 0 ), 1.0f );
        Assert::AreEqual( mat( 1 ), 0.0f );
        Assert::AreEqual( mat( 2, 2 ), 1.0f );

        Maths::Matrix4 mat2;
        mat2( 0 ) = 5.5f;
        Assert::AreEqual( mat2[0], 5.5f );
    }

    // TODO: Rotations

};

// TODO: Vector tests