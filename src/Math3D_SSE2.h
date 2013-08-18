#ifndef MATH_2D_SSE2
#define MATH_2D_SSE2
#include <Config.h>
#include <Math3D.h>

#if defined( SIMD_SSE2 )
namespace Easy3D {
    //matrix * line
    extern  __m128 SSE2_lincomb(const __m128 &a, const Easy3D::Matrix4x4 &B);
    // Multiplies two 4x4 matrices (a,b) auto=a*b
    extern void SSE2_Matrix4Mul(Easy3D::Matrix4x4 &out,
                                const Easy3D::Matrix4x4 &A,
                                const Easy3D::Matrix4x4 &B);
    extern void SSE2_Matrix4Inv(Matrix4x4& self);
    extern void SSE2_Matrix4Transpose(Matrix4x4& self);
}
#endif

#endif
