/********************************************************************
	filename: 	PANDUVector2
	author:		Parag Moni Boro
	
	purpose:	Game Engine created for learning
*********************************************************************/

#ifndef __PANDUVector2_h__
#define __PANDUVector2_h__

#include <Math.h>
#include <limits>

namespace Pandu
{
	class Vector2 
	{
	public:

		union
		{
			struct 
			{
				float x;
				float y;
			};
			float arr[2];
		};

	public:

		inline Vector2()
		{
		}
		
		inline Vector2(float _x, float _y)
			: x(_x)
			, y(_y)
		{
		}
		
		inline Vector2(const Vector2& _src)
			: x(_src.x)
			, y(_src.y)
		{
		}

		inline Vector2& operator = ( const Vector2& _right )
		{
			x = _right.x;
			y = _right.y;
			return *this;
		}
		
		inline const Vector2& operator + () const
		{
			return *this;
		}

		inline Vector2 operator - () const
		{
			return Vector2(-x,-y);
		}
		
		inline Vector2 operator + ( const Vector2& _right ) const
		{
			return Vector2( x + _right.x, y + _right.y);
		}

		inline Vector2& operator += ( const Vector2& _right )
		{
			x += _right.x;
			y += _right.y;
			return *this;
		}
		
		inline Vector2 operator - ( const Vector2& _right ) const
		{
			return Vector2( x - _right.x, y - _right.y);
		}

		inline Vector2& operator -= ( const Vector2& _right )
		{
			x -= _right.x;
			y -= _right.y;
			return *this;
		}
		
		inline Vector2 operator * ( float _right ) const
		{
			return Vector2( x * _right, y * _right);
		}

		inline Vector2& operator *= ( float _right )
		{
			x *= _right;
			y *= _right;
			return *this;
		}
		
		inline Vector2 operator / ( float _right ) const
		{
			return Vector2( x / _right, y / _right);
		}

		inline Vector2& operator /= ( float _right )
		{
			x /= _right;
			y /= _right;
			return *this;
		}
		
		inline Vector2 ComponentMultiply( const Vector2& _right ) const
		{
			return Vector2(x * _right.x, y * _right.y);
		}
		
		inline Vector2 ComponentDivide( const Vector2& _right ) const
		{
			return Vector2(x / _right.x, y / _right.y);
		}
		
		inline bool operator == ( const Vector2& _right ) const
		{
			if( x == _right.x && y == _right.y )
			{
				return true;
			}
			return false;
		}
		
		inline bool operator != ( const Vector2& _right ) const
		{
			if( x != _right.x || y != _right.y)
			{
				return true;
			}
			return false;
		}
		
		inline float SqrdLength() const
		{
			return (x * x + y * y );
		}
		
		inline float Length() const
		{
			return (float)sqrt(SqrdLength());
		}
		
		inline float Distance(const Vector2& _target) const
		{
			return (*this - _target).Length();
		}
		
		inline float Dot(const Vector2& _right) const
		{
			return x * _right.x + y * _right.y;
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
		
		inline Vector2 MidPoint( const Vector2& _toVec )
		{
			return Vector2( ( x + _toVec.x ) * 0.5f , ( y + _toVec.y ) * 0.5f );
		}

		//-----------------------------------------------------------------------------
		//Friend functions

        inline friend Vector2 operator * ( const float _scaler , const Vector2& _right )
        {
            return Vector2(_scaler * _right.x,_scaler * _right.y);
        }
		//-----------------------------------------------------------------------------
		
		static const Vector2 UNIT_X;
		static const Vector2 UNIT_Y;
		static const Vector2 NEGATIVE_UNIT_X;
		static const Vector2 NEGATIVE_UNIT_Y;
		static const Vector2 ZERO;
		static const Vector2 UNIT;
	};
}

#endif