/********************************************************************
	filename: 	PANDUSkeletalAnimation
	author:		Parag Moni Boro
	
	purpose:	Game Engine created for learning
*********************************************************************/

#ifndef __PANDUSkeletalAnimation_h__
#define __PANDUSkeletalAnimation_h__

#include "PANDUNodeAnimation.h"
#include "../boost/smart_ptr.hpp"
#include <map>

namespace Pandu
{
	class SkeletalAnimation;

	typedef boost::shared_ptr<SkeletalAnimation> TSharedSkeletalAnimationPtr;
	typedef boost::weak_ptr<SkeletalAnimation> TSkeletalAnimationWeakPtr;
	
	typedef std::map<const String, TSharedSkeletalAnimationPtr> TNameSharedSkeletanAnimationMap;

	class SkeletalAnimation : public Animation
	{
	protected:

		friend class Skeleton;

		const unsigned int m_BoneCount;
		NodeAnimation** m_AllBoneAnims;
		const unsigned int m_FrameCount;
		
		SkeletalAnimation(const String& _name, unsigned int _boneCount, unsigned int _frameCount);
	
		void InitializeAllToNull();
		
	public:

		~SkeletalAnimation();

		// always frames are needed to add in ascending order
		void SetAllFramesDataWithDataOwnership(unsigned int _boneIndex, NodeAnimationFrame* _allFrames, unsigned int _dataCount);

		const NodeAnimation* GetNodeAnimation(unsigned int _boneIndex) const;

		//inline float GetTotalTime() const;
	};
}

#endif //__PANDUSkeletalAnimation_h__