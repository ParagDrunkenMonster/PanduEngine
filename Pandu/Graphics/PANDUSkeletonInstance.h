/********************************************************************
	filename: 	PANDUSkeletonInstance
	author:		Parag Moni Boro
	
	purpose:	Game Engine created for learning
*********************************************************************/

#ifndef __PANDUSkeletonInstance_h__
#define __PANDUSkeletonInstance_h__

#include "../boost/smart_ptr.hpp"
#include <list>
#include "PANDUBone.h"
#include "PANDUSkeletalAnimation.h"

namespace Pandu
{
	class SkeletonInstance;
	
	typedef boost::shared_ptr<SkeletonInstance> TSharedSkeletonInstancePtr;
	typedef boost::weak_ptr<SkeletonInstance> TSkeletonInstanceWeakPtr;

	class SkeletonInstance
	{
	protected:

		friend class SkeletonMeshModifier;
		friend class SkeletalAnimationBlender;

		SkeletonInstance(){}
		virtual ~SkeletonInstance(){}

		virtual const TBoneSharedPtrArray& GetBonesSharedPtrArray() const = 0;

	public:

		virtual void Update() = 0;
		virtual TBoneWeakPtr GetRootBone() const = 0;
		virtual TBoneWeakPtr GetBoneByIndex(unsigned int _index) = 0;
		virtual int GetChildCountOfBone(unsigned int _boneIndex) const = 0;
		virtual int GetBoneIdOfChildOfBone(unsigned int _boneIndex, unsigned int _childIndex) const = 0;
		virtual unsigned int BoneCount() const = 0;
		virtual const TSharedSkeletalAnimationPtr GetAnimation(const String& _name) const = 0;
		virtual const String& SkeletonName() const = 0;
	};
}

#endif