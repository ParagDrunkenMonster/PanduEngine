/********************************************************************
	filename: 	PANDUVector3
	author:		Parag Moni Boro
	
	purpose:	Game Engine created for learning
*********************************************************************/

#ifndef __PANDUVector3_h__
#define __PANDUVector3_h__

#include <Math.h>
#include <limits>
#include "PANDUMathConstants.h"

namespace Pandu
{
	class Vector3 
	{
	public:

		union
		{
			struct 
			{
				float x;
				float y;
				float z;
			};
			float arr[3];
		};

	public:

		inline Vector3()
		{
		}
		
		inline Vector3(float _x, float _y, float _z)
			: x(_x)
			, y(_y)
			, z(_z)
		{
		}
		
		inline Vector3(const Vector3& _src)
			: x(_src.x)
			, y(_src.y)
			, z(_src.z)
		{
		}

		float* GetArray() {	return &(arr[0]);	}

		inline Vector3& operator = ( const Vector3& _right )
		{
			x = _right.x;
			y = _right.y;
			z = _right.z;
			return *this;
		}
		
		inline const Vector3& operator + () const
		{
			return *this;
		}

		inline Vector3 operator - () const
		{
			return Vector3(-x,-y,-z);
		}
		
		inline Vector3 operator + ( const Vector3& _right ) const
		{
			return Vector3( x + _right.x, y + _right.y, z + _right.z);
		}

		inline Vector3& operator += ( const Vector3& _right )
		{
			x += _right.x;
			y += _right.y;
			z += _right.z;
			return *this;
		}
		
		inline Vector3 operator - ( const Vector3& _right ) const
		{
			return Vector3( x - _right.x, y - _right.y, z - _right.z);
		}

		inline Vector3& operator -= ( const Vector3& _right )
		{
			x -= _right.x;
			y -= _right.y;
			z -= _right.z;
			return *this;
		}
		
		inline Vector3 operator * ( float _right ) const
		{
			return Vector3( x * _right, y * _right, z * _right);
		}

		inline Vector3& operator *= ( float _right )
		{
			x *= _right;
			y *= _right;
			z *= _right;
			return *this;
		}
		
		inline Vector3 operator / ( float _right ) const
		{
			return Vector3( x / _right, y / _right, z / _right);
		}

		inline Vector3& operator /= ( float _right )
		{
			x /= _right;
			y /= _right;
			z /= _right;
			return *this;
		}
		
		inline Vector3 ComponentMultiply( const Vector3& _right ) const
		{
			return Vector3(x * _right.x, y * _right.y, z * _right.z);
		}
		
		inline Vector3 ComponentDivide( const Vector3& _right ) const
		{
			return Vector3(x / _right.x, y / _right.y, z / _right.z);
		}
		
		inline bool operator == ( const Vector3& _right ) const
		{
			if( x == _right.x && y == _right.y && z == _right.z )
			{
				return true;
			}
			return false;
		}
		
		inline bool operator != ( const Vector3& _right ) const
		{
			if( x != _right.x || y != _right.y || z != _right.z )
			{
				return true;
			}
			return false;
		}
		
		inline float SqrdLength() const
		{
			return (x * x + y * y + z * z);
		}
		
		inline float Length() const
		{
			return (float)sqrt(SqrdLength());
		}
		
		inline float Distance(const Vector3& _target) const
		{
			return (*this - _target).Length();
		}
		
		inline float Dot(const Vector3& _right) const
		{
			return x * _right.x + y * _right.y + z * _right.z;
		}
		
		inline Vector3 Cross(const Vector3& _right) const
		{
			return Vector3(	  y * _right.z - z * _right.y
							, z * _right.x - x * _right.z
							, x * _right.y - y * _right.x );
		}
		
		// returns true if normalized and otherwise false
		inline bool Normalize()
		{
			 float length = Length();

			 if ( length > (std::numeric_limits<float>::epsilon() ) )
			 {
				 float invLength = 1.0f / length;
				 *this *= invLength;
				 return true;
			 }
	         
			 return false;
		}
		
		inline Vector3 MidPoint( const Vector3& _toVec )
		{
			return Vector3( ( x + _toVec.x ) * 0.5f , ( y + _toVec.y ) * 0.5f , ( z + _toVec.z ) * 0.5f );
		}

		 /** Returns true if this vector is zero length. */
        inline bool IsZeroLength(void) const
        {
            float sqlen = (x * x) + (y * y) + (z * z);
            return (sqlen < (1e-06 * 1e-06));
        }
		
		//-----------------------------------------------------------------------------
		//Friend functions

        inline friend Vector3 operator * ( const float _scaler , const Vector3& _right )
        {
            return Vector3(_scaler * _right.x,_scaler * _right.y,_scaler * _right.z);
        }
		//-----------------------------------------------------------------------------

		
   
		
		static const Vector3 UNIT_X;
		static const Vector3 UNIT_Y;
		static const Vector3 UNIT_Z;
		static const Vector3 NEGATIVE_UNIT_X;
		static const Vector3 NEGATIVE_UNIT_Y;
		static const Vector3 NEGATIVE_UNIT_Z;
		static const Vector3 ZERO;
		static const Vector3 UNIT;
	};
}

#endif