#include <stdafx.h>
#include "Math3D_SSE2.h"

#if defined( SIMD_SSE2 )
namespace Easy3D{

ALIGNED(16,
union SSE2Struct {
	__m128 smid;
	ALIGNED(16, float v[4]);

	DFORCEINLINE SSE2Struct(){
		smid=_mm_setzero_ps();
	}
	DFORCEINLINE SSE2Struct(const float *a){
		v[0]=a[0];
		v[1]=a[1];
		v[2]=a[2];
		v[3]=a[3];
	}
	DFORCEINLINE void toVec(float *a){
		a[0]=v[0];
		a[1]=v[1];
		a[2]=v[2];
		a[3]=v[3];
	}

});
    
// linear combination:
// a[0] * B.row[0] + a[1] * B.row[1] + a[2] * B.row[2] + a[3] * B.row[3]
extern __m128 SSE2_lincomb(const __m128 &a, const Easy3D::Matrix4x4 &B)
{
    __m128 result;
    result = _mm_mul_ps(_mm_shuffle_ps(a, a, 0x00), B.row0);
    result = _mm_add_ps(result, _mm_mul_ps(_mm_shuffle_ps(a, a, 0x55), B.row1));
    result = _mm_add_ps(result, _mm_mul_ps(_mm_shuffle_ps(a, a, 0xaa), B.row2));
    result = _mm_add_ps(result, _mm_mul_ps(_mm_shuffle_ps(a, a, 0xff), B.row3));
    return result;
}
    
extern void SSE2_Matrix4Mul(Matrix4x4 &out,
                            const Matrix4x4 &A,
                            const Matrix4x4 &B)
{

    __m128 out0x = SSE2_lincomb(A.row0, B);
    __m128 out1x = SSE2_lincomb(A.row1, B);
    __m128 out2x = SSE2_lincomb(A.row2, B);
    __m128 out3x = SSE2_lincomb(A.row3, B);
    
    out.row0 = out0x;
    out.row1 = out1x;
    out.row2 = out2x;
    out.row3 = out3x;
}

extern void SSE2_Matrix4Inv(Matrix4x4& self){
    
__m128 self_row0 = _mm_load_ps(self.entries);
__m128 self_row1 = _mm_load_ps(self.entries+4);
__m128 self_row2 = _mm_load_ps(self.entries+8);
__m128 self_row3 = _mm_load_ps(self.entries+12);
 __m128 minor0, minor1, minor2, minor3;
 __m128 det, tmp1;
// -----------------------------------------------
 tmp1 = _mm_mul_ps(self_row2, self_row3);
 tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0xB1);
 minor0 = _mm_mul_ps(self_row1, tmp1);
 minor1 = _mm_mul_ps(self_row0, tmp1);
 tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0x4E);
 minor0 = _mm_sub_ps(_mm_mul_ps(self_row1, tmp1), minor0);
 minor1 = _mm_sub_ps(_mm_mul_ps(self_row0, tmp1), minor1);
 minor1 = _mm_shuffle_ps(minor1, minor1, 0x4E);
// -----------------------------------------------
 tmp1 = _mm_mul_ps(self_row1, self_row2);
 tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0xB1);
 minor0 = _mm_add_ps(_mm_mul_ps(self_row3, tmp1), minor0);
 minor3 = _mm_mul_ps(self_row0, tmp1);
 tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0x4E);
 minor0 = _mm_sub_ps(minor0, _mm_mul_ps(self_row3, tmp1));
 minor3 = _mm_sub_ps(_mm_mul_ps(self_row0, tmp1), minor3);
 minor3 = _mm_shuffle_ps(minor3, minor3, 0x4E);
// -----------------------------------------------
 tmp1 = _mm_mul_ps(_mm_shuffle_ps(self_row1, self_row1, 0x4E), self_row3);
 tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0xB1);
 self_row2 = _mm_shuffle_ps(self_row2, self_row2, 0x4E);
 minor0 = _mm_add_ps(_mm_mul_ps(self_row2, tmp1), minor0);
 minor2 = _mm_mul_ps(self_row0, tmp1);
 tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0x4E);
 minor0 = _mm_sub_ps(minor0, _mm_mul_ps(self_row2, tmp1));
 minor2 = _mm_sub_ps(_mm_mul_ps(self_row0, tmp1), minor2);
 minor2 = _mm_shuffle_ps(minor2, minor2, 0x4E);
// -----------------------------------------------
 tmp1 = _mm_mul_ps(self_row0, self_row1);//Streaming SIMD Extensions - Inverse of 4x4 Matrix
 tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0xB1);
 minor2 = _mm_add_ps(_mm_mul_ps(self_row3, tmp1), minor2);
 minor3 = _mm_sub_ps(_mm_mul_ps(self_row2, tmp1), minor3);
 tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0x4E);
 minor2 = _mm_sub_ps(_mm_mul_ps(self_row3, tmp1), minor2);
 minor3 = _mm_sub_ps(minor3, _mm_mul_ps(self_row2, tmp1));
// -----------------------------------------------
 tmp1 = _mm_mul_ps(self_row0, self_row3);
 tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0xB1);
 minor1 = _mm_sub_ps(minor1, _mm_mul_ps(self_row2, tmp1));
 minor2 = _mm_add_ps(_mm_mul_ps(self_row1, tmp1), minor2);
 tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0x4E);
 minor1 = _mm_add_ps(_mm_mul_ps(self_row2, tmp1), minor1);
 minor2 = _mm_sub_ps(minor2, _mm_mul_ps(self_row1, tmp1));
// -----------------------------------------------
 tmp1 = _mm_mul_ps(self_row0, self_row2);
 tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0xB1);
 minor1 = _mm_add_ps(_mm_mul_ps(self_row3, tmp1), minor1);
 minor3 = _mm_sub_ps(minor3, _mm_mul_ps(self_row1, tmp1));
 tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0x4E);
 minor1 = _mm_sub_ps(minor1, _mm_mul_ps(self_row3, tmp1));
 minor3 = _mm_add_ps(_mm_mul_ps(self_row1, tmp1), minor3);
// -----------------------------------------------
 det = _mm_mul_ps(self_row0, minor0);
 det = _mm_add_ps(_mm_shuffle_ps(det, det, 0x4E), det);
 det = _mm_add_ss(_mm_shuffle_ps(det, det, 0xB1), det);
 tmp1 = _mm_rcp_ss(det);
 det = _mm_sub_ss(_mm_add_ss(tmp1, tmp1), _mm_mul_ss(det, _mm_mul_ss(tmp1, tmp1)));
 det = _mm_shuffle_ps(det, det, 0x00);
 //
 minor0 = _mm_mul_ps(det, minor0);
 _mm_storel_pi((__m64*)(self.entries), minor0);
 _mm_storeh_pi((__m64*)(self.entries+2), minor0);
 minor1 = _mm_mul_ps(det, minor1);
 _mm_storel_pi((__m64*)(self.entries+4), minor1);
 _mm_storeh_pi((__m64*)(self.entries+6), minor1);
 minor2 = _mm_mul_ps(det, minor2);
 _mm_storel_pi((__m64*)(self.entries+ 8), minor2);
 _mm_storeh_pi((__m64*)(self.entries+10), minor2);
 minor3 = _mm_mul_ps(det, minor3);
 _mm_storel_pi((__m64*)(self.entries+12), minor3);
 _mm_storeh_pi((__m64*)(self.entries+14), minor3);

}

};

#endif
