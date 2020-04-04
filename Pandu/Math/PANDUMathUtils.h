/********************************************************************
	filename: 	PANDUMathUtils
	author:		Parag Moni Boro
	
	purpose:	Game Engine created for learning
*********************************************************************/

#ifndef __PANDUMathUtils_h__
#define __PANDUMathUtils_h__

#include "PANDUMathConstants.h"

namespace Pandu
{
	static float Sign(float _val)
	{
		return (_val >= 0.0f?1.0f:-1.0f);
	}

	static float Abs(float _val)
	{
		return (_val >=0.0f?_val:-_val);
	}

	static void Clamp(float& _inOutVal, float _min,float _max)
	{
		if(_inOutVal < _min)
		{
			_inOutVal = _min;
		}
		
		if(_inOutVal > _max)
		{
			_inOutVal = _max;
		}
	}

	static float Max(float _val1, float _val2)
	{
		return (_val1 > _val2) ? _val1 : _val2;
	}

	static float Min(float _val1, float _val2)
	{
		return (_val1 > _val2) ? _val2 : _val1;
	}

	static float GetRadiansFromDegrees(float _degrees)
	{
		return ((kPI/180.0f) * _degrees);
	}
}

#endif