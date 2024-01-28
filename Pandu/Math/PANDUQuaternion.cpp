#include "PANDUQuaternion.h"
#include "PANDUMatrix44.h"
#include <cmath>

namespace Pandu
{
    const Quaternion Quaternion::ZERO(0.0,0.0,0.0,0.0);
    const Quaternion Quaternion::IDENTITY(1.0,0.0,0.0,0.0);

	//-----------------------------------------------------------------------
    Vector3 Quaternion::operator * (const Vector3& _right) const
    {
		Vector3 uv, uuv;
		Vector3 qvec(x, y, z);
		uv = qvec.Cross(_right);
		uuv = qvec.Cross(uv);
		uv *= (2.0f * w);
		uuv *= 2.0f;

		return _right + uv + uuv;

    }

    //-----------------------------------------------------------------------
    void Quaternion::FromAngleAxis (float _angle, const Vector3& _axis)
    {
        // The quaternion representing the rotation is
        // q = cos(A/2)+sin(A/2)*(x*i+y*j+z*k)

        float halfAngle ( 0.5f * _angle );
        float fSin = sin(halfAngle);
        w = cos(halfAngle);
        x = fSin * _axis.x;
        y = fSin * _axis.y;
        z = fSin * _axis.z;
    }
 
    //-----------------------------------------------------------------------
    Quaternion Quaternion::Inverse () const
    {
        float sqrdLen = w * w + x * x + y * y + z * z;

        if ( sqrdLen > (std::numeric_limits<float>::epsilon() ) )
        {
            float invSqrdLen = 1.0f/sqrdLen;
            return Quaternion( w * invSqrdLen , - x * invSqrdLen , -y * invSqrdLen , -z * invSqrdLen);
        }
        else
        {
            return ZERO;
        }
	}

	//-----------------------------------------------------------------------
	void Quaternion::ToRotationMatrix(Matrix44& _oRot) const
    {
        float fTx  = x+x;
        float fTy  = y+y;
        float fTz  = z+z;
        float fTwx = fTx*w;
        float fTwy = fTy*w;
        float fTwz = fTz*w;
        float fTxx = fTx*x;
        float fTxy = fTy*x;
        float fTxz = fTz*x;
        float fTyy = fTy*y;
        float fTyz = fTz*y;
        float fTzz = fTz*z;

        _oRot[0][0] = 1.0f-(fTyy+fTzz);
        _oRot[0][1] = fTxy-fTwz;
        _oRot[0][2] = fTxz+fTwy;
		_oRot[0][3] = 0;
        _oRot[1][0] = fTxy+fTwz;
        _oRot[1][1] = 1.0f-(fTxx+fTzz);
        _oRot[1][2] = fTyz-fTwx;
		_oRot[1][3] = 0;
        _oRot[2][0] = fTxz-fTwy;
        _oRot[2][1] = fTyz+fTwx;
        _oRot[2][2] = 1.0f-(fTxx+fTyy);
		_oRot[2][3] = 0;

		_oRot[3][0] = 0;
        _oRot[3][1] = 0;
        _oRot[3][2] = 0;
		_oRot[3][3] = 1.0f;
    }

	//calculations taken from Ogre
	void Quaternion::FromRotationMatrix( const Matrix33& _rotMat )
	{
		float fTrace = _rotMat.m[0][0] + _rotMat.m[1][1] + _rotMat.m[2][2];
		float froot;

		if ( fTrace > 0.0 )
		{
			// |w| > 1/2, may as well choose w > 1/2
			froot = sqrt( fTrace + 1.0f);  // 2w
			w = 0.5f * froot;
			froot = 0.5f / froot;  // 1/(4w)
			x = ( _rotMat.m[2][1] - _rotMat.m[1][2] ) * froot;
			y = ( _rotMat.m[0][2] - _rotMat.m[2][0] ) * froot;
			z = ( _rotMat.m[1][0] - _rotMat.m[0][1] ) * froot;
		}
		else
		{
			// |w| <= 1/2
			static const unsigned short s_iNext[3] = { 1, 2, 0 };
			unsigned short i = 0;
			if ( _rotMat.m[1][1] > _rotMat.m[0][0] )
			{
				i = 1;
			}
			
			if ( _rotMat.m[2][2] > _rotMat.m[i][i] )
			{
				i = 2;
			}
			unsigned short j = s_iNext[i];
			unsigned short k = s_iNext[j];

			froot = sqrt( _rotMat.m[i][i] - _rotMat.m[j][j] - _rotMat.m[k][k] + 1.0f );
			float* apkQuat[3] = { &x, &y, &z };
			*apkQuat[i] = 0.5f * froot;
			froot = 0.5f / froot;
			w = ( _rotMat.m[k][j]-_rotMat.m[j][k] ) * froot;
			*apkQuat[j] = ( _rotMat.m[j][i] + _rotMat.m[i][j] ) * froot;
			*apkQuat[k] = ( _rotMat.m[k][i] + _rotMat.m[i][k] ) * froot;
		}
	}

	//-----------------------------------------------------------------------
	Quaternion Quaternion::Slerp (const Quaternion& _start, const Quaternion& _end, float _paramT, bool shortestPath)
	{
		float startDotEnd = _start.Dot(_end);
		Quaternion realEnd;

		if (startDotEnd < 0.0f && shortestPath)
		{
			startDotEnd = -startDotEnd;
			realEnd = -_end;
		}
		else
		{
			realEnd = _end;
		}

		if ( (float)abs(startDotEnd) < 1.0f - (std::numeric_limits<float>::epsilon() ) )
		{
			// Standard case (slerp)
			float fSin = sqrt(1.0f - (startDotEnd * startDotEnd));
			float fAngle = atan2f(fSin, startDotEnd);
			float fInvSin = 1.0f / fSin;
			float fCoeff0 = sin((1.0f - _paramT) * fAngle) * fInvSin;
			float fCoeff1 = sin(_paramT * fAngle) * fInvSin;
			
			return fCoeff0 * _start + fCoeff1 * realEnd;
		}
		
		// There are two situations:
		// 1. "_start" and "_end" are very close (startDotEnd ~= +1), so we can do a linear
		//    interpolation safely.
		// 2. "_start" and "_end" are almost inverse of each other (startDotEnd ~= -1), there
		//    are an infinite number of possibilities interpolation. but we haven't
		//    have method to fix this case, so just use linear interpolation here.
		Quaternion t = (1.0f - _paramT) * _start + _paramT * realEnd;
		// taking the complement requires re normalization
		t.Normalize();
		
		return t;
	}

	//-----------------------------------------------------------------------
	Quaternion Quaternion::Lerp(const Quaternion& _start, const Quaternion& _end, float _paramT, bool shortestPath)
	{
		Quaternion result;

		float dot = _start.Dot(_end);
		if (dot < 0.0f && shortestPath)
		{
			result = _start + ((-_end) - _start) * _paramT;
		}
		else
		{
			result = _start + (_end - _start) * _paramT;
		}
		result.Normalize();
		return result;
	}
}
