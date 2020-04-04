/********************************************************************
	filename: 	PANDUSkeletalAnimationBlender
	author:		Parag Moni Boro
	
	purpose:	Game Engine created for learning
*********************************************************************/

#ifndef __PANDUSkeletalAnimationBlender_h__
#define __PANDUSkeletalAnimationBlender_h__

#include "PANDUSkeletalAnimationController.h"
#include "PANDUSkeletonInstance.h"

namespace Pandu
{
	class SkeletalAnimationBlender;

	typedef boost::shared_ptr<SkeletalAnimationBlender> TSkeletalAnimationBlenderSharedPtr;

	class SkeletalAnimationBlender
	{
	protected:

		typedef std::pair<TSharedSkeletalAnimationControllerPtr,TSharedSkeletalAnimationPtr> TControllerAnimationPair;
		typedef std::vector<TControllerAnimationPair> TAnimationsPlayingArray;

		struct PreservedTransform
		{
			Vector3 m_Position;
			Quaternion m_Rotation;
		};

		TSharedSkeletonInstancePtr m_SkeletonInst;
		TAnimationsPlayingArray m_AllControllers;
		unsigned int m_MaxControllers;
		float m_BlendSpeed;
		float m_CurrBlendFactor;
		PreservedTransform *m_AllPreservedTransforms;
	
	public:
	
		SkeletalAnimationBlender(TSharedSkeletonInstancePtr _skeletonInst, unsigned int _maxControllers = 2);
		virtual ~SkeletalAnimationBlender();

		inline void SetMaxControllers(unsigned int _maxControllers)		{	m_MaxControllers = _maxControllers;		}

		//bool AddAnimation()

		//if blend speed is <= 0.0f it will not blend with previous animation
		TSharedSkeletalAnimationControllerPtr SetAnimation(TSkeletalAnimationControllerAutoPtr _anim, float _blendSpeed = 0.0f);

		bool IsAllAnimationsFinished() const;

		virtual void Update(float _deltaTime);

	private:

		void InitializePreservedTransforms();

		void ClearAllAnimations();
		void DestroyAllPreservedTransforms();

		void PreserveTransforms();

		void SetAnimToSkeleton(TControllerAnimationPair& _anim);
	};
}

#endif //__PANDUSkeletalAnimationBlender_h__