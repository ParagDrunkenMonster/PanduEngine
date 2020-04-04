#include "PANDUSkeletalAnimationController.h"

namespace Pandu
{
	SkeletalAnimationController::~SkeletalAnimationController()
	{
	}

	void SkeletalAnimationController::Update(float _deltaTime)
	{
		if( !m_DirtyTime && _deltaTime != 0.0f )
		{
			m_CurrAnimTime += _deltaTime;
			m_DirtyTime = true;
		}
		
	}

	void SkeletalAnimationController::SetBoneCount(unsigned int _boneCount)
	{
		m_BoneCount = _boneCount;
	}

	void SkeletalAnimationController::UpdateTimes(const float* _times, unsigned int _frameCount)
	{
		if( !m_DirtyTime )
		{
			return;
		}

		const float lastFrameTime = _times[_frameCount - 1];

		if( m_CurrFrame >= ((int)_frameCount - 1) || m_CurrAnimTime >= lastFrameTime )
		{
			if( m_ControllerType != eCT_Loop )
			{
				m_CurrFrame = (int)_frameCount - 1;
				m_CurrAnimTime = lastFrameTime;
				m_CurrFrameInterpolateValue = 0.0f;
				m_AnimationFinished = true;
				return;
			}
			else
			{
				const unsigned int multiplier = 10000000;//if m_CurrAnimTime is too big than the lastFrameTime,
				//animation will keep looping in one single frame,
				//to eliminate that we just need to find the 
				//m_CurrAnimTime%lastFrameTime
				//i don't know if there is any other way to find it
				//but this works good enough for me
				const unsigned int intCurrAnimTime = (unsigned int)(m_CurrAnimTime * multiplier);
				const unsigned int intLastAnimTime = (unsigned int)(lastFrameTime * multiplier);
				m_CurrAnimTime = ((float)(intCurrAnimTime%intLastAnimTime))/multiplier;
				m_CurrFrame = 0;
			}
		}

		while(m_CurrAnimTime >= _times[m_CurrFrame+1])
		{
			m_CurrFrame++;
			if( m_CurrFrame >= ((int)_frameCount - 2) )
			{
				break;
			}
		}

		const float currTime = _times[m_CurrFrame];
		const float nextTime = _times[m_CurrFrame+1];
		m_CurrFrameInterpolateValue = ( m_CurrAnimTime - nextTime )/(nextTime - currTime);
	}

	void SkeletalAnimationController::GetAnimTransformCurrFrame(const NodeAnimation& _nodeAnim, unsigned int _boneInd, Vector3& _oPos, Quaternion& _oRot)
	{
		if( !m_DirtyTime )
		{
			return;
		}

		PANDU_ERROR(_boneInd < m_BoneCount,"Bone index is more than Bone Count");
		const unsigned int _frameCount = _nodeAnim.GetFrameCount();
		
		PANDU_ERROR(m_CurrFrame < (int)_frameCount,"Animation has unequal frames for bones");

		const NodeAnimationFrame* lastFrame = _nodeAnim.GetFrame(_frameCount - 1);

		if( m_CurrFrame >= ((int)_frameCount - 1) )
		{
			if( m_ControllerType != eCT_Loop )
			{
				_oPos = lastFrame->m_Position;
				_oRot = lastFrame->m_Rotation;
				return;
			}
		}

		const NodeAnimationFrame* frame = _nodeAnim.GetFrame(m_CurrFrame);
		const NodeAnimationFrame* nextFrame = _nodeAnim.GetFrame(m_CurrFrame + 1);

		switch(nextFrame->m_InterpolateMode)
		{
		case Interpolator::eIM_Linear:
			{
				LinearInterpolator interpolater;
				_oPos = interpolater.Interpolate(frame->m_Position,nextFrame->m_Position,m_CurrFrameInterpolateValue);
				_oRot = interpolater.Interpolate(frame->m_Rotation,nextFrame->m_Rotation,m_CurrFrameInterpolateValue);
			}
			break;

		default:
			{

			}
			break;
		}
	}
}