#include "PANDUSkeletonDebugObject.h"

namespace Pandu
{
	SkeletonDebugObject::SkeletonDebugObject(TSkeletonInstanceWeakPtr _skeleton)
		: m_SkeletonInstance(_skeleton)
		, m_NearPlaneSize(0.01f)
	{
		
	}

	SkeletonDebugObject::~SkeletonDebugObject()
	{
		m_SkeletonInstance.reset();
	}

	void SkeletonDebugObject::Render()
	{
		TSharedSkeletonInstancePtr sklInstance = m_SkeletonInstance.lock();
		TSharedDebugDrawerPtr debugDrawer = m_DebugDrawer.lock();
		TSharedFrustrumPtr frustrum = m_FrustrumPtr.lock();

		if( sklInstance != NULL && debugDrawer != NULL && frustrum != NULL )
		{	
			TSharedBonePtr rootBone = sklInstance->GetRootBone().lock();			
			DrawSkeleton(sklInstance,debugDrawer,frustrum,rootBone);
		}
	}

	void SkeletonDebugObject::DrawSkeleton(TSharedSkeletonInstancePtr _skelInst, TSharedDebugDrawerPtr _debugDrawer, TSharedFrustrumPtr _frustrum, TSharedBonePtr _root)
	{
		DrawBone(_debugDrawer,_frustrum,_root->GetWorldTransform());
		const int childCount = _skelInst->GetChildCountOfBone(_root->GetIndex());

		for( int i = 0 ; i < childCount ; i++ )
		{
			const int child = _skelInst->GetBoneIdOfChildOfBone(_root->GetIndex(),(unsigned int)i);
			PANDU_ERROR( child >= 0, "Unable to find bone index in skeleton");
			TSharedBonePtr childBone = _skelInst->GetBoneByIndex(child).lock();
			PANDU_ERROR( childBone != NULL , "Unable to find bone in skeleton");
			DrawBoneLink(_debugDrawer,_frustrum,_root->GetWorldTransform(),childBone->GetWorldTransform());
			DrawSkeleton(_skelInst,_debugDrawer,_frustrum,childBone);
		}
	}

	void SkeletonDebugObject::DrawBone( TSharedDebugDrawerPtr _debugDrawer, TSharedFrustrumPtr _frustrum, const Matrix44& _boneTransform )
	{
		const float radius = GetRadius(_frustrum,_boneTransform,m_NearPlaneSize);
		Colour boneColor(0.0f,1.0f,0.0f,1.0f);
		_debugDrawer->DrawSphere(_boneTransform.GetTranslate() + Vector3(0.0f,6.0f,0.0f),radius,9,9,boneColor);
	}

	void SkeletonDebugObject::DrawBoneLink(TSharedDebugDrawerPtr _debugDrawer, TSharedFrustrumPtr _frustrum, const Matrix44& _parentTransform, const Matrix44& _childTransform )
	{
		const Vector3 start = _parentTransform.GetTranslate() + Vector3(0.0f,6.0f,0.0f);
		const Vector3 end = _childTransform.GetTranslate() + Vector3(0.0f,6.0f,0.0f);

		Vector3 dirVec = end - start;//z
		
		if( dirVec.SqrdLength() > 0.0f )
		{
			dirVec.Normalize();
			const float startRadius = GetRadius(_frustrum,_parentTransform,m_NearPlaneSize);

			const Vector3 centerPos = start + dirVec * startRadius;

			Vector3 perp1 = _parentTransform.GetColumnVec3(0);//x
			Vector3 perp2 = dirVec.Cross(perp1);//y
			if( perp2.SqrdLength() < 0.0001f )
			{
				perp1 = _parentTransform.GetColumnVec3(1);//x
				perp2 = dirVec.Cross(perp1);//y
			}
			perp1 = perp2.Cross(dirVec);

			perp1.Normalize();
			perp2.Normalize();

			const float distFromCamera = (_frustrum->GetInverseViewMatrix().GetTranslate() - centerPos).Length();

			const float radiusNearPlane = m_NearPlaneSize;
			const float halfWidth = radiusNearPlane * distFromCamera * _frustrum->GetTanHalfFOVY();

			Vector3 topLeft =		centerPos - halfWidth * perp1 + halfWidth * perp2;
			Vector3 topRight =		centerPos + halfWidth * perp1 + halfWidth * perp2;
			Vector3 bottomLeft =	centerPos - halfWidth * perp1 - halfWidth * perp2;
			Vector3 bottomRight =	centerPos + halfWidth * perp1 - halfWidth * perp2;

			Colour lineColour(1.0f,1.0f,1.0f,1.0f);

			_debugDrawer->DrawLine(topLeft,topRight,lineColour);
			_debugDrawer->DrawLine(topRight,bottomRight,lineColour);
			_debugDrawer->DrawLine(bottomRight,bottomLeft,lineColour);
			_debugDrawer->DrawLine(bottomLeft,topLeft,lineColour);

			_debugDrawer->DrawLine(topLeft,bottomRight,lineColour);
			_debugDrawer->DrawLine(topRight,bottomLeft,lineColour);

			const float endRadius = GetRadius(_frustrum,_childTransform,m_NearPlaneSize);
			const Vector3 endPos = end - dirVec * endRadius;

			_debugDrawer->DrawLine(topLeft,endPos,lineColour);
			_debugDrawer->DrawLine(topRight,endPos,lineColour);
			_debugDrawer->DrawLine(bottomLeft,endPos,lineColour);
			_debugDrawer->DrawLine(bottomRight,endPos,lineColour);

		}
	}

	float SkeletonDebugObject::GetRadius(TSharedFrustrumPtr _frustrum, const Matrix44& _boneTransform, float _nearPlaneSize)
	{
		const float distFromCamera = (_frustrum->GetInverseViewMatrix().GetTranslate() - _boneTransform.GetTranslate()).Length();

		const float radiusNearPlane = _nearPlaneSize;
		const float radius = radiusNearPlane * distFromCamera * _frustrum->GetTanHalfFOVY();

		return radius;
	}

	bool SkeletonDebugObject::CanDelete() const
	{
		TSharedSkeletonInstancePtr sklInstance = m_SkeletonInstance.lock();
		if( sklInstance == NULL)
		{
			return true;
		}

		return false;
	}
}