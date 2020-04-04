/********************************************************************
	filename: 	PANDUQuaternion
	author:		Parag Moni Boro
	
	purpose:	Game Engine created for learning
*********************************************************************/

#ifndef __PANDUQuaternion_h__
#define __PANDUQuaternion_h__

#include "PANDUVector3.h"
#include "PANDUMatrix33.h"
#include <assert.h>
#include <Math.h>

namespace Pandu 
{
	class Matrix44;

    class Quaternion
    {
	public:

		float w, x, y, z;

    public:

		inline Quaternion()
		{
		}

        inline Quaternion (float _W , float _X , float _Y , float _Z )
			: w(_W)
			, x(_X)
			, y(_Y)
			, z(_Z)
		{
		}
       
        inline Quaternion(float _angRadian, const Vector3& _axis)
        {
            this->FromAngleAxis(_angRadian, _axis);
        } 

		inline Quaternion(const Matrix33& _rotMat)
		{
			this->FromRotationMatrix(_rotMat);
		}

		inline Quaternion& operator = (const Quaternion& _right)
		{
			w = _right.w;
			x = _right.x;
			y = _right.y;
			z = _right.z;
			return *this;
		}

		inline void Set(float _W , float _X , float _Y , float _Z)
		{
			w = _W;
			x = _X;
			y = _Y;
			z = _Z;
		}

		inline Quaternion operator + (const Quaternion& _right) const
		{
			return Quaternion(w + _right.w , x + _right.x , y + _right.y , z + _right.z);
		}

        inline Quaternion operator - (const Quaternion& _right) const
		{
			return Quaternion(w - _right.w , x - _right.x , y - _right.y , z - _right.z);
		}

        inline Quaternion operator * (const Quaternion& _right) const
		{
			//p * q != q * p
			return Quaternion (
								w * _right.w - x * _right.x - y * _right.y - z * _right.z,
								w * _right.x + x * _right.w + y * _right.z - z * _right.y,
								w * _right.y + y * _right.w + z * _right.x - x * _right.z,
								w * _right.z + z * _right.w + x * _right.y - y * _right.x
								);
		}
        
		inline Quaternion operator * (float _right) const
		{
			return Quaternion(_right * w, _right * x , _right * y , _right * z );
		}

        inline Quaternion operator - () const
		{
			return Quaternion(-w,-x,-y,-z);
		}

        inline bool operator == (const Quaternion& _right) const
		{
			return (_right.x == x) && (_right.y == y) && (_right.z == z) && (_right.w == w);
		}

        inline bool operator != (const Quaternion& _right) const
		{
			return !operator == (_right);
		}

		
		inline float operator [] ( short _i ) const
		{
			assert( _i < 4 );
			return *( &w + _i );
		}

		inline float& operator [] ( short _i )
		{
			assert( _i < 4 );
			return *( &w + _i );
		}

        inline float Dot(const Quaternion& _quat) const
		{
			return w * _quat.w + x * _quat.x + y * _quat.y + z * _quat.z;
		}

        inline float SqrdLength() const
		{
			return w * w + x * x + y * y + z * z;
		}

        inline bool Normalize()
		{
			float sqrdlen = SqrdLength();

			if( sqrdlen > (std::numeric_limits<float>::epsilon() ) )
			{
				float factor = 1.0f / sqrt(sqrdlen);
				*this = *this * factor;
				return true;
			}

			return false;
		}

		Vector3 operator * (const Vector3& _right) const;

		void FromRotationMatrix( const Matrix33& _rotMat );
        void FromAngleAxis(float _angle, const Vector3& _axis);        
        Quaternion Inverse() const;
		void ToRotationMatrix (Matrix44& _oRot) const;

		//-----------------------------------------------------------------------------
		//Friend functions
		friend inline Quaternion operator * (float _left , const Quaternion& _right)
		{
			return Quaternion(_left * _right.w , _left * _right.x , _left * _right.y , _left * _right.z);
		}

		//-----------------------------------------------------------------------------
		//Static functions
		static Quaternion Slerp(const Quaternion& _start, const Quaternion& _end, float _paramT, bool shortestPath);
		static Quaternion Lerp(const Quaternion& _start, const Quaternion& _end, float _paramT, bool shortestPath);

		//-----------------------------------------------------------------------------

        static const Quaternion ZERO;
        static const Quaternion IDENTITY;
    };
}

#endif