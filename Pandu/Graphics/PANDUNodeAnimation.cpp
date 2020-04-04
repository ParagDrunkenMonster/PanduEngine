#include "PANDUNodeAnimation.h"

namespace Pandu
{
	NodeAnimation::~NodeAnimation()
	{
		ClearAllFramesData();
	}

	// always frames are needed to add in ascending order
	NodeAnimationFrame* NodeAnimation::GetFrame(unsigned int _index)
	{
		PANDU_ERROR(m_AllFrames && GetFrameCount() > _index , "Very few frames for this animation");
		return &m_AllFrames[_index];
	}

	const NodeAnimationFrame* NodeAnimation::GetFrame(unsigned int _index) const
	{
		PANDU_ERROR(m_AllFrames && GetFrameCount() > _index , "Very few frames for this animation");
		return &m_AllFrames[_index];
	}

	void NodeAnimation::SetAllFramesDataWithDataOwnership(NodeAnimationFrame* _allFrames, unsigned int _dataCount)
	{
		PANDU_ERROR(m_FrameCount == _dataCount , "Data count is not equal to frame count");
		m_AllFrames = _allFrames;
	}

	void NodeAnimation::ClearAllFramesData()
	{
		if(m_AllFrames)
		{
			delete [] m_AllFrames;
			m_AllFrames = NULL;
		}
		if(m_AllTimes)
		{
			delete [] m_AllTimes;
			m_AllTimes = NULL;
		}
	}
}