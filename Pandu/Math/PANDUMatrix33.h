#/********************************************************************
	filename: 	PANDUMatrix33
	author:		Parag Moni Boro
	
	purpose:	Game Engine created for learning
*********************************************************************/

#ifndef __PANDUMatrix33_h__
#define __PANDUMatrix33_h__

#include "PANDUVector3.h"
#include <assert.h>

namespace Pandu
{
	class Matrix33
	{
	public:

		union 
		{
            float m[3][3];
            float arr[9];
        };

		inline Matrix33(){}
        inline Matrix33 (const Matrix33& _right)
		{
			memcpy( m, _right.m, 9 * sizeof(float) );
		}

		inline Matrix33(const Vector3& _xAxis, const Vector3& _yAxis, const Vector3& _zAxis)
		{
			this->FromAxes(_xAxis,_yAxis,_zAxis);
		}

        inline Matrix33(float _fEntry00, float _fEntry01, float _fEntry02,
                float _fEntry10, float _fEntry11, float _fEntry12,
                float _fEntry20, float _fEntry21, float _fEntry22)
		{
			m[0][0] = _fEntry00;		m[0][1] = _fEntry01;		m[0][2] = _fEntry02;
			m[1][0] = _fEntry10;		m[1][1] = _fEntry11;		m[1][2] = _fEntry12;
			m[2][0] = _fEntry20;		m[2][1] = _fEntry21;		m[2][2] = _fEntry22;
		}

        inline Vector3 GetColumnVec3(unsigned short _colInd) const
		{
			assert( _colInd < 3 );
			return Vector3(m[0][_colInd],m[1][_colInd],m[2][_colInd]);
		}

        inline void SetColumnVec3(unsigned short _colInd, const Vector3& _vec)
		{
			assert( _colInd < 3 );
			m[0][_colInd] = _vec.x;	m[1][_colInd] = _vec.y;	m[2][_colInd] = _vec.z;
		}

        inline void FromAxes(const Vector3& _xAxis, const Vector3& _yAxis, const Vector3& _zAxis)
		{
			SetColumnVec3(0,_xAxis);
			SetColumnVec3(1,_yAxis);
			SetColumnVec3(2,_zAxis);
		}

        // assignment and comparison
        inline Matrix33& operator = (const Matrix33& _right)
		{
			memcpy( m, _right.m ,9 * sizeof(float) );
			return *this;
		}
       
        // arithmetic operations
        inline Matrix33 operator + (const Matrix33& _right) const
		{
			Matrix33 sum;
			for( unsigned short rowCount = 0; rowCount < 3; rowCount++ )
			{
				for( unsigned short colCount = 0; colCount < 3; colCount++ )
				{
					sum.m[rowCount][colCount] = m[rowCount][colCount] + _right.m[rowCount][colCount];
				}
			}
			return sum;
		}

        Matrix33 operator - (const Matrix33& _right) const
		{
			Matrix33 diff;
			for( unsigned short rowCount = 0; rowCount < 3; rowCount++ )
			{
				for( unsigned short colCount = 0; colCount < 3; colCount++ )
				{
					diff.m[rowCount][colCount] = m[rowCount][colCount] - _right.m[rowCount][colCount];
				}
			}
			return diff;
		}

        Matrix33 operator * (const Matrix33& _right) const
		{
			Matrix33 product;
			for( unsigned short rowCount = 0; rowCount < 3; rowCount++ )
			{
				for( unsigned short colCount = 0; colCount < 3; colCount++ )
				{
					product.m[rowCount][colCount] =	m[rowCount][0] * _right.m[0][colCount] + m[rowCount][1] * _right.m[1][colCount] + m[rowCount][2] * _right.m[2][colCount];
				}
			}
			return product;
		}
        
		inline Matrix33 operator - () const
		{
			Matrix33 neg;
			for( unsigned short rowCount = 0; rowCount < 3; rowCount++ )
			{
				for( unsigned short colCount = 0; colCount < 3; colCount++ )
				{
					neg.m[rowCount][colCount] = -m[rowCount][colCount];
				}
			}
			return neg;
		}

        // matrix * vector [3x3 * 3x1 = 3x1]
        inline Vector3 operator * (const Vector3& _vec ) const
		{
			Vector3 product;
			for( unsigned short rowCount = 0; rowCount < 3; rowCount++ )
			{
				product.arr[rowCount] = m[rowCount][0] * _vec.arr[0] + m[rowCount][1] * _vec.arr[1] + m[rowCount][2] * _vec.arr[2];
			}
			return product;
		}

        // vector * matrix [1x3 * 3x3 = 1x3]
        friend inline Vector3 operator * (const Vector3& _left, const Matrix33& _right)
		{
			Vector3 product;
			for( unsigned short rowCount = 0; rowCount < 3; rowCount++ )
			{
				product.arr[rowCount] = _left.arr[0] * _right.m[0][rowCount] + _left.arr[1] * _right.m[1][rowCount] + _left.arr[2] * _right.m[2][rowCount];
			}
			return product;
		}

        inline Matrix33 Transpose () const
		{
			Matrix33 transpose;
			for( unsigned short rowCount = 0; rowCount < 3; rowCount++ )
			{
				for( unsigned short colCount = 0; colCount < 3; colCount++ )
				{
					transpose.m[rowCount][colCount] = m[colCount][rowCount];
				}
			}
			return transpose;
		}

		static const Matrix33 ZERO;
        static const Matrix33 IDENTITY;       

	};
}

#endif