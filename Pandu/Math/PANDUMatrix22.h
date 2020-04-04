/********************************************************************
	filename: 	PANDUMatrix22
	author:		Parag Moni Boro
	
	purpose:	Game Engine created for learning
*********************************************************************/

#ifndef __PANDUMatrix22_h__
#define __PANDUMatrix22_h__

#include "PANDUVector2.h"
#include <assert.h>

namespace Pandu
{

	//   R =  cos(t) -sin(t) 
	//        sin(t)  cos(t)
	// where t > 0 indicates a counterclockwise rotation in the xy-plane.

    class Matrix22
    {
	private:
		union
		{
			float m[2][2];
			float array[4];
		};

    public:
 
		inline Matrix22(){}

		inline Matrix22(float _radAngle)
		{
			SetRotation(_radAngle);
		}

        inline Matrix22 (const Matrix22& _right)
		{
			memcpy(array,_right.array,4 * sizeof(float));
		}

        inline Matrix22 ( float _v00, float _v01, float _v10, float _v11)
		{
			Set(_v00, _v01, _v10, _v11 );
		}

		inline void Set(float _v00, float _v01, float _v10, float _v11)
		{
			m[0][0] = _v00;		m[0][1] = _v01;	
			m[1][0] = _v10;		m[1][1] = _v11;
		}

        // member access, allows use of construct mat[r][c]
        inline float* operator[] (short _Row)
		{
			assert( _Row < 2 );
			return (float*)m[_Row];
		}

		inline const float *operator [] ( short _Row ) const
		{
			assert( _Row < 2 );
			return (float*)m[_Row];
		}

        
        inline Matrix22& operator = (const Matrix22& _right)
		{
			memcpy(array, _right.array, 4 * sizeof(float) );
			return *this;
		}

        // arithmetic operations
        inline Matrix22 operator + (const Matrix22& _right) const
		{
			Matrix22 result;
			result.array[0] = array[0] + _right.array[0];		result.array[1] = array[0] + _right.array[1];
			result.array[2] = array[0] + _right.array[2];		result.array[3] = array[0] + _right.array[3];
			
			return result;
		}

        inline Matrix22 operator - (const Matrix22& _right) const
		{
			Matrix22 result;
			result.array[0] = array[0] - _right.array[0];		result.array[1] = array[0] - _right.array[1];
			result.array[2] = array[0] - _right.array[2];		result.array[3] = array[0] - _right.array[3];
			
			return result;
		}

        inline Matrix22 Matrix22::operator * (const Matrix22& _right) const
		{
			Matrix22 prod;

			prod.m[0][0] = m[0][0] * _right.m[0][0] + m[0][1] * _right.m[1][0];
			prod.m[0][1] = m[0][0] * _right.m[0][1] + m[0][1] * _right.m[1][1];
			prod.m[1][0] = m[1][0] * _right.m[0][0] + m[1][1] * _right.m[1][0];
			prod.m[1][1] = m[1][0] * _right.m[0][1] + m[1][1] * _right.m[1][1];

			return prod;
		}
		//-----------------------------------------------------------------------
		inline Vector2 Matrix22::operator * (const Vector2& _right) const
		{
			Vector2 prod;

			prod.x = m[0][0] * _right.x + m[0][1] * _right.y;
			prod.y = m[1][0] * _right.x + m[1][1] * _right.y;

			return prod;
		}

		inline Matrix22 operator - () const
		{
			Matrix22 kNeg;
			kNeg.array[0] = -array[0];
			kNeg.array[1] = -array[1];
			kNeg.array[2] = -array[2];
			kNeg.array[3] = -array[3];
			return kNeg;
		}

        // matrix * scalar
        inline Matrix22 operator * (float _scalar) const
		{
			Matrix22 result;
			result.array[0] = _scalar * array[0];
			result.array[1] = _scalar * array[1];
			result.array[2] = _scalar * array[2];
			result.array[3] = _scalar * array[3];
			return result;
		}

        // scalar * matrix
        friend Matrix22 operator * (float _scalar, const Matrix22& _right)
		{
			Matrix22 result;
			result.array[0] = _scalar * _right.array[0];
			result.array[1] = _scalar * _right.array[1];
			result.array[2] = _scalar * _right.array[2];
			result.array[3] = _scalar * _right.array[3];
			return result;
		}

        // utilities
        inline Matrix22 GetTranspose () const
		{
			return Matrix22(m[0][0], m[1][0], m[0][1], m[1][1]);
        }

		inline void Transpose()
        {
            Set(m[0][0], m[1][0], m[0][1], m[1][1]);
		}

		inline void SetRotation(float _radAngle)
		{
			m[0][0] = cos(_radAngle);		m[0][1] = -sin(_radAngle);
			m[1][0] = sin(_radAngle);		m[1][1] =  cos(_radAngle);
		}

        static const Matrix22 ZERO;
        static const Matrix22 IDENTITY;
    };
}

#endif