/********************************************************************
	filename: 	PANDUVector4
	author:		Parag Moni Boro
	
	purpose:	Game Engine created for learning
*********************************************************************/

#ifndef __PANDUVector4_h__
#define __PANDUVector4_h__

#include <Math.h>
#include <limits>
#include "PANDUMathConstants.h"

namespace Pandu
{
	class Vector4
	{
	public:

		union
		{
			struct 
			{
				float x;
				float y;
				float z;
				float w;
			};
			float arr[4];
		};

	public:

		inline Vector4()
		{
		}

		inline Vector4(float _x, float _y, float _z, float _w)
			: x(_x)
			, y(_y)
			, z(_z)
			, w(_w)
		{
		}

		inline Vector4(const Vector4& _src)
			: x(_src.x)
			, y(_src.y)
			, z(_src.z)
			, w(_src.w)
		{
		}

		float* GetArray() {	return &(arr[0]);	}

		inline Vector4& operator = ( const Vector4& _right )
		{
			x = _right.x;
			y = _right.y;
			z = _right.z;
			w = _right.w;
			return *this;
		}

		inline const Vector4& operator + () const
		{
			return *this;
		}

		inline Vector4 operator - () const
		{
			return Vector4(-x,-y,-z,-w);
		}

		inline Vector4 operator + ( const Vector4& _right ) const
		{
			return Vector4( x + _right.x, y + _right.y, z + _right.z, w + _right.w );
		}

		inline Vector4& operator += ( const Vector4& _right )
		{
			x += _right.x;
			y += _right.y;
			z += _right.z;
			w += _right.w;
			return *this;
		}

		inline Vector4 operator - ( const Vector4& _right ) const
		{
			return Vector4( x - _right.x, y - _right.y, z - _right.z, w - _right.w);
		}

		inline Vector4& operator -= ( const Vector4& _right )
		{
			x -= _right.x;
			y -= _right.y;
			z -= _right.z;
			w -= _right.w;
			return *this;
		}

		inline Vector4 operator * ( float _right ) const
		{
			return Vector4( x * _right, y * _right, z * _right, w * _right );
		}

		inline Vector4& operator *= ( float _right )
		{
			x *= _right;
			y *= _right;
			z *= _right;
			w *= _right;
			return *this;
		}

		inline Vector4 operator / ( float _right ) const
		{
			return Vector4( x / _right, y / _right, z / _right, w / _right );
		}

		inline Vector4& operator /= ( float _right )
		{
			x /= _right;
			y /= _right;
			z /= _right;
			w /= _right;
			return *this;
		}

		inline Vector4 ComponentMultiply( const Vector4& _right ) const
		{
			return Vector4(x * _right.x, y * _right.y, z * _right.z, w * _right.w );
		}

		inline Vector4 ComponentDivide( const Vector4& _right ) const
		{
			return Vector4(x / _right.x, y / _right.y, z / _right.z, w / _right.w );
		}

		inline bool operator == ( const Vector4& _right ) const
		{
			if( x == _right.x && y == _right.y && z == _right.z && w == _right.w )
			{
				return true;
			}
			return false;
		}

		inline bool operator != ( const Vector4& _right ) const
		{
			if( x != _right.x || y != _right.y || z != _right.z || w != _right.w )
			{
				return true;
			}
			return false;
		}

		inline float SqrdLength() const
		{
			return (x * x + y * y + z * z + w * w);
		}

		inline float Length() const
		{
			return (float)sqrt(SqrdLength());
		}

		inline float Distance(const Vector4& _target) const
		{
			return (*this - _target).Length();
		}

		inline float Dot(const Vector4& _right) const
		{
			return x * _right.x + y * _right.y + z * _right.z + w * _right.w;
		}

		/*inline Vector4 Cross(const Vector4& _right) const
		{
			return Vector4(	  y * _right.z - z * _right.y
				, z * _right.x - x * _right.z
				, x * _right.y - y * _right.x );
		}*/

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

		/** Returns true if this vector is zero length. */
		inline bool IsZeroLength(void) const
		{
			float sqlen = (x * x) + (y * y) + (z * z);
			return (sqlen < (1e-06 * 1e-06));
		}

		//-----------------------------------------------------------------------------
		//Friend functions

		inline friend Vector4 operator * ( const float _scaler , const Vector4& _right )
		{
			return Vector4(_scaler * _right.x,_scaler * _right.y,_scaler * _right.z,_scaler * _right.w);
		}
		//-----------------------------------------------------------------------------


		static const Vector4 UNIT_X;
		static const Vector4 UNIT_Y;
		static const Vector4 UNIT_Z;
		static const Vector4 UNIT_W;
		static const Vector4 NEGATIVE_UNIT_X;
		static const Vector4 NEGATIVE_UNIT_Y;
		static const Vector4 NEGATIVE_UNIT_Z;
		static const Vector4 NEGATIVE_UNIT_W;
		static const Vector4 ZERO;
		static const Vector4 UNIT;
	};
}

#endif //__PANDUVector4_h__