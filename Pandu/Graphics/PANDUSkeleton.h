/********************************************************************
	filename: 	PANDUSkeleton
	author:		Parag Moni Boro
	
	purpose:	Game Engine created for learning
*********************************************************************/

#ifndef __PANDUSkeleton_h__
#define __PANDUSkeleton_h__

#include "../Math/PANDUMatrix33.h"
#include "../Math/PANDUMatrix44.h"
#include "../Utils/PANDUCoreDefines.h"
#include "PANDUSkeletonInstance.h"
#include <vector>
#include <map>

namespace Pandu
{
	class Skeleton;
	
	typedef boost::shared_ptr<Skeleton> TSharedSkeletonPtr;
	typedef boost::weak_ptr<Skeleton> TSkeletonWeakPtr;

	typedef std::list<TSharedSkeletonPtr> TSharedSkeletonPtrList;
	typedef std::map<const String,TSharedSkeletonPtr> TSharedSkeletonMap;

	class Skeleton
	{
	public:

		struct BoneInfo
		{
			String m_Name;
			unsigned int m_BoneIndex;
			Matrix44 m_BoneMatrix;
		};

	private:

		class Impl;
		boost::scoped_ptr<Impl> m_Impl;

	public:
	
		Skeleton(const String& _name);
		virtual ~Skeleton();

		// returns the index of the bone
		// returns -1 if an error occurred
		int CreateBoneInfo(const String& _boneName, const Vector3& _bonePos, const Quaternion& _boneRotation = Quaternion::IDENTITY);
		int CreateBoneInfo(const String& _boneName, const Matrix44& _boneMatrix);
		void AddChildToBone(unsigned int _boneIndex, unsigned int _childIndex);

		TSharedSkeletonInstancePtr CreateSkeletonInstance() const;

		TSharedSkeletalAnimationPtr CreateAnimation(const String& _animName, unsigned int _frameCount);
		TSharedSkeletalAnimationPtr GetAnimation(const String& _name);

		const String& GetName() const;

		inline int GetBoneIndex(const String& _boneName ) const
		{
			const BoneInfo* info = GetBoneInfoByName(_boneName);
			return (info?info->m_BoneIndex:-1);
		}

		unsigned int GetBoneCount() const;

	protected:

		const BoneInfo* GetBoneInfoByName(const String& _boneName) const;
	};
}

#endif //__PANDUSkeleton_h__