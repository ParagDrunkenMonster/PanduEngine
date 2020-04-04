/********************************************************************
	filename: 	PANDUAnimation
	author:		Parag Moni Boro
	
	purpose:	Game Engine created for learning
*********************************************************************/

#ifndef __PANDUAnimation_h__
#define __PANDUAnimation_h__

#include "Utils/PANDUString.h"
#include "../Utils/PANDUCoreDefines.h"

namespace Pandu
{
	class Animation
	{
	public:

		enum AnimationType
		{
			eAT_Node,
			eAT_Skeletal,
		};

	protected:

		const String m_AnimName;
		const AnimationType m_AnimationType;
		const unsigned int m_FrameCount;
		float *m_AllTimes;
	
		inline Animation(AnimationType _animType, const String& _name, unsigned int _frameCount)
			: m_AnimationType(_animType)
			, m_AnimName(_name)
			, m_FrameCount(_frameCount)
			, m_AllTimes(NULL)
		{

		}

	public:

		virtual ~Animation()
		{
			if(m_AllTimes)
			{
				delete [] m_AllTimes;
				m_AllTimes = NULL;
			}
		}

		const String& GetName() const						{	return m_AnimName;			}
		inline unsigned int GetFrameCount() const			{	return m_FrameCount;		}

		inline void SetAllTimesDataWithDataOwnership(float* _allTimes, unsigned int _dataCount)
		{
			PANDU_ERROR(m_FrameCount == _dataCount , "Data count is not equal to frame count");
			m_AllTimes = _allTimes;
		}

		inline const float* GetAllTimes() const					{	return m_AllTimes;			}

	};
}

#endif //__PANDUAnimation_h__