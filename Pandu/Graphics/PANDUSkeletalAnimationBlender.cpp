#include "PANDUSkeletalAnimationBlender.h"

namespace Pandu
{
	SkeletalAnimationBlender::SkeletalAnimationBlender(TSharedSkeletonInstancePtr _skeletonInst, unsigned int _maxControllers)
		: m_SkeletonInst(_skeletonInst)
		, m_MaxControllers(_maxControllers)
		, m_BlendSpeed(0.0f)
		, m_CurrBlendFactor(1.0f)
		, m_AllPreservedTransforms(NULL)
	{
		InitializePreservedTransforms();
	}

	SkeletalAnimationBlender::~SkeletalAnimationBlender()
	{
		m_SkeletonInst.reset();
		ClearAllAnimations();
		DestroyAllPreservedTransforms();
	}

	void SkeletalAnimationBlender::InitializePreservedTransforms()
	{
		if( !m_AllPreservedTransforms )
		{
			m_AllPreservedTransforms = new PreservedTransform[m_SkeletonInst->BoneCount()];
		}
	}

	void SkeletalAnimationBlender::ClearAllAnimations()
	{
		m_AllControllers.clear();
	}

	void SkeletalAnimationBlender::DestroyAllPreservedTransforms()
	{
		if( m_AllPreservedTransforms )
		{
			delete [] m_AllPreservedTransforms;
			m_AllPreservedTransforms = NULL;
		}
	}

	void SkeletalAnimationBlender::PreserveTransforms()
	{
		const TBoneSharedPtrArray& bones = m_SkeletonInst->GetBonesSharedPtrArray();
		const unsigned int boneCount = (unsigned int)bones.size();

		for( unsigned int i = 0 ; i < boneCount ; i++ )
		{
			m_AllPreservedTransforms[i].m_Position = bones[i]->GetPosition();
			m_AllPreservedTransforms[i].m_Rotation = bones[i]->GetRotation();
		}
	}

	TSharedSkeletalAnimationControllerPtr SkeletalAnimationBlender::SetAnimation(TSkeletalAnimationControllerAutoPtr _anim, float _blendSpeed)
	{
		const String& animName = _anim->GetAnimationName();
		const TSharedSkeletalAnimationPtr anim = m_SkeletonInst->GetAnimation(animName);

		if( anim != NULL )
		{
			const unsigned int animCount = (unsigned int)m_AllControllers.size();

			if( animCount == 0 || _blendSpeed <= 0.0f )
			{
				_blendSpeed = 0.0f;
				m_CurrBlendFactor = 1.0f;
			}
			else
			{
				PreserveTransforms();
				m_BlendSpeed = _blendSpeed;
				m_CurrBlendFactor = 0.0f;
				ClearAllAnimations();
			}

			TSharedSkeletalAnimationControllerPtr sharedAnimController(_anim.release());
			m_AllControllers.push_back(TControllerAnimationPair(sharedAnimController,anim));

			sharedAnimController->SetWeight(1.0f);
			sharedAnimController->SetBoneCount(m_SkeletonInst->BoneCount());
			sharedAnimController->SetDirtyTime(true);

			return sharedAnimController;
		}

		gLogManager->WriteWarningLog("No animation called '" + animName + "' in skeleton '" + m_SkeletonInst->SkeletonName() + "'");
		return TSharedSkeletalAnimationControllerPtr();
	}

	void SkeletalAnimationBlender::Update(float _deltaTime)
	{
		const unsigned int animCount = (unsigned int)m_AllControllers.size();
		if( animCount == 0)
		{
			return;
		}

		m_CurrBlendFactor += m_BlendSpeed * _deltaTime;
		if( m_CurrBlendFactor > 1.0f && m_BlendSpeed > 0.0f )
		{
			_deltaTime = (m_CurrBlendFactor - 1.0f)/m_BlendSpeed;
			m_CurrBlendFactor = 1.0f;
		}

		if( m_CurrBlendFactor < 1.0f )
		{
			for( unsigned int i = 0 ; i < animCount ; i++ )
			{
				m_AllControllers[i].first->Update(0.0f);
			}
		}
		else
		{
			for( unsigned int i = 0 ; i < animCount ; i++ )
			{
				m_AllControllers[i].first->Update(_deltaTime);
			}
		}

		for( unsigned int i = 0 ; i < animCount ; i++ )
		{
			if( !m_AllControllers[i].first->IsAnimationFinished() )
			{
				SetAnimToSkeleton(m_AllControllers[i]);
			}
		}

		m_SkeletonInst->Update();
	}

	void SkeletalAnimationBlender::SetAnimToSkeleton(TControllerAnimationPair& _anim)
	{
		_anim.first->UpdateTimes(_anim.second->GetAllTimes(),_anim.second->GetFrameCount());

		const TBoneSharedPtrArray& allBones = m_SkeletonInst->GetBonesSharedPtrArray();
		const unsigned int boneCount = m_SkeletonInst->BoneCount();
		for( unsigned int i = 0 ; i < boneCount ; i++ )
		{
			Vector3 outPos;
			Quaternion outRot;
			unsigned int outCurrFrame = 0;
			const NodeAnimation* nodeAnim = _anim.second->GetNodeAnimation(i);

			if(nodeAnim)
			{
				_anim.first->GetAnimTransformCurrFrame(*nodeAnim,i, outPos, outRot);
				allBones[i]->SetRotation(outRot);
				allBones[i]->SetPosition(outPos);
			}
		}

		_anim.first->SetDirtyTime(false);
	}

	bool SkeletalAnimationBlender::IsAllAnimationsFinished() const
	{
		const unsigned int animCount = (unsigned int)m_AllControllers.size();
		for( unsigned int i = 0 ; i < animCount ; i++ )
		{
			if( !m_AllControllers[i].first->IsAnimationFinished() )
			{
				return false;
			}
		}
		return true;
	}
}