/********************************************************************
	filename: 	PANDUSkeletalAnimationController
	author:		Parag Moni Boro
	
	purpose:	Game Engine created for learning
*********************************************************************/

#ifndef __PANDUSkeletalAnimationController_h__
#define __PANDUSkeletalAnimationController_h__

#include "PANDUAnimationController.h"
#include "PANDUNodeAnimation.h"
#include "PANDUInterpolator.h"
#include "../boost/smart_ptr.hpp"
#include <vector>

namespace Pandu
{
	class SkeletalAnimationController;

	typedef boost::shared_ptr<SkeletalAnimationController> TSharedSkeletalAnimationControllerPtr;
	typedef boost::weak_ptr<SkeletalAnimationController> TSkeletalAnimationControllerWeakPtr;
	typedef std::auto_ptr<SkeletalAnimationController> TSkeletalAnimationControllerAutoPtr;

	typedef std::vector<TSharedSkeletalAnimationControllerPtr> TSharedSkeletalAnimationControllerPtrArray;

	class SkeletalAnimationController : public AnimationController
	{
	protected:

		friend class SkeletalAnimationBlender;

		const String m_AnimName;
		float m_CurrAnimTime;
		unsigned int m_BoneCount;
		int m_CurrFrame;
		float m_Weight;
		float m_CurrFrameInterpolateValue;
		bool m_AnimationFinished;
		bool m_DirtyTime;
	
	public:
	
		inline SkeletalAnimationController(ControllerType _controllerType, const String& _animName)
			: AnimationController(_controllerType)
			, m_AnimName(_animName)
			, m_CurrAnimTime(0.0f)
			, m_BoneCount(0)
			, m_CurrFrame(0)
			, m_Weight(1.0f)
			, m_CurrFrameInterpolateValue(0.0f)
			, m_AnimationFinished(false)
			, m_DirtyTime(false)
		{

		}
		virtual ~SkeletalAnimationController();

		inline const String& GetAnimationName() const				{	return m_AnimName;			}
		inline float GetCurrentTime() const							{	return m_CurrAnimTime;		}
		inline unsigned int GetCurrentFrame() const					{	return m_CurrFrame;			}

		inline void SetWeight(float _weight)						{	m_Weight = _weight;			}
		inline float GetWeight() const								{	return m_Weight;			}

		inline bool IsAnimationFinished() const						{	return m_AnimationFinished;	}

		virtual void Update(float _deltaTime);

	private:
		
		inline void SetDirtyTime(bool _dirtyTime)					{	m_DirtyTime = _dirtyTime;	}
		void SetBoneCount(unsigned int _boneCount);
		void UpdateTimes(const float* _times, unsigned int _frameCount);
		void GetAnimTransformCurrFrame(const NodeAnimation& _nodeAnim, unsigned int _boneInd, Vector3& _oPos, Quaternion& _oRot);
		
	};
}

#endif //__PANDUSkeletalAnimationController_h__