#include "PANDUSkeletalAnimation.h"

namespace Pandu
{
	SkeletalAnimation::SkeletalAnimation(const String& _name, unsigned int _boneCount, unsigned int _frameCount)
		: Animation(eAT_Skeletal,_name,_frameCount)
		, m_BoneCount(_boneCount)
		, m_AllBoneAnims(new NodeAnimation*[_boneCount])
		, m_FrameCount(_frameCount)
	{
		InitializeAllToNull();
	}

	SkeletalAnimation::~SkeletalAnimation()
	{
		for( unsigned int i = 0 ; i < m_BoneCount ; i++ )
		{
			if( m_AllBoneAnims[i] )
			{
				delete m_AllBoneAnims[i];
				m_AllBoneAnims[i] = NULL;
			}
		}
		delete [] m_AllBoneAnims;
		m_AllBoneAnims = NULL;
	}

	void SkeletalAnimation::InitializeAllToNull()
	{
		for( unsigned int i = 0 ; i < m_BoneCount ; i++ )
		{
			m_AllBoneAnims[i] = NULL;
		}
	}

	void SkeletalAnimation::SetAllFramesDataWithDataOwnership(unsigned int _boneIndex, NodeAnimationFrame* _allFrames, unsigned int _dataCount)
	{
		PANDU_ERROR(_boneIndex < m_BoneCount && m_FrameCount == _dataCount, "Bone index has to be less than bone count");
		if( !m_AllBoneAnims[_boneIndex] )
		{
			m_AllBoneAnims[_boneIndex] = new NodeAnimation("",m_FrameCount);
		}

		m_AllBoneAnims[_boneIndex]->SetAllFramesDataWithDataOwnership(_allFrames,_dataCount);
	}

	const NodeAnimation* SkeletalAnimation::GetNodeAnimation(unsigned int _boneIndex) const
	{
		PANDU_ERROR(_boneIndex < m_BoneCount, "Bone index has to be less than bone count");
		return m_AllBoneAnims[_boneIndex];
	}


}