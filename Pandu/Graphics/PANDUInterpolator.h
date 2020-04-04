/********************************************************************
	filename: 	PANDUInterpolator
	author:		Parag Moni Boro
	
	purpose:	Game Engine created for learning
*********************************************************************/

#ifndef __PANDUInterpolator_h__
#define __PANDUInterpolator_h__

#include "Math/PANDUVector3.h"
#include "Math/PANDUQuaternion.h"

namespace Pandu
{
	class Interpolator
	{
	public:

		enum InterpolationMode
		{
			eIM_Linear = 0,
			eIM_Count,
		};

	protected:
	
		inline Interpolator(){}
		virtual ~Interpolator(){}

	public:

		virtual Vector3 Interpolate(const Vector3& _start, const Vector3& _end, float _paramT) = 0;
		virtual Quaternion Interpolate(const Quaternion& _start, const Quaternion& _end, float _paramT) = 0;
	};

	class LinearInterpolator : public Interpolator
	{
	public:

		LinearInterpolator():Interpolator(){}
		~LinearInterpolator(){}

		virtual Vector3 Interpolate(const Vector3& _start, const Vector3& _end, float _paramT)
		{
			return (_start + (_end - _start) * _paramT);
		}

		virtual Quaternion Interpolate(const Quaternion& _start, const Quaternion& _end, float _paramT)
		{
			//TODO: need to optimize using linear interpolation
			return Quaternion::Lerp(_start,_end,_paramT,false);
		}

	};
}

#endif //__PANDUInterpolator_h__