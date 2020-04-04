/********************************************************************
	filename: 	PANDUNodeAnimation
	author:		Parag Moni Boro
	
	purpose:	Game Engine created for learning
*********************************************************************/

#ifndef __PANDUNodeAnimation_h__
#define __PANDUNodeAnimation_h__

#include "PANDUInterpolator.h"
#include "PANDUAnimation.h"
#include "boost/assert.hpp"

namespace Pandu
{
	struct NodeAnimationFrame
	{
		Interpolator::InterpolationMode m_InterpolateMode;
		Vector3 m_Position;
		Quaternion m_Rotation;
	};

	class NodeAnimation : public Animation
	{
	protected:

		NodeAnimationFrame* m_AllFrames;
	
	public:
	
		inline NodeAnimation(const String& _animName, unsigned int _frameCount)
			: Animation(eAT_Node,_animName,_frameCount)
			, m_AllFrames(NULL)
		{

		}

		virtual ~NodeAnimation();

		void SetAllFramesDataWithDataOwnership(NodeAnimationFrame* _allFrames, unsigned int _dataCount);
		
		NodeAnimationFrame* GetFrame(unsigned int _index);
		const NodeAnimationFrame* GetFrame(unsigned int _index) const;
		inline const NodeAnimationFrame* GetAllFrames() const		{	return m_AllFrames;			}
		
	protected:

		void ClearAllFramesData();
	};
}

#endif //__PANDUNodeAnimation_h__