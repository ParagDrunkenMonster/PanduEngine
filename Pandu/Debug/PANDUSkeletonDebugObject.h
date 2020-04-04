/********************************************************************
	filename: 	PANDUSkeletonDebugObject
	author:		Parag Moni Boro
	
	purpose:	Game Engine created for learning
*********************************************************************/

#ifndef __PANDUSkeletonDebugObject_h__
#define __PANDUSkeletonDebugObject_h__

#include "../Graphics/PANDUSkeletonInstance.h"
#include "PANDUDebugObject.h"

namespace Pandu
{
	class SkeletonDebugObject : public DebugObject
	{
	private:

		TSkeletonInstanceWeakPtr m_SkeletonInstance;
		float m_NearPlaneSize;
	
	public:
	
		SkeletonDebugObject(TSkeletonInstanceWeakPtr _skeleton);
		~SkeletonDebugObject();

		virtual void Render();

	private:

		virtual void DebugDrawerUpdated(){};
		bool CanDelete() const;

		void DrawSkeleton(TSharedSkeletonInstancePtr _skelInst, TSharedDebugDrawerPtr _debugDrawer, TSharedFrustrumPtr _frustrum, TSharedBonePtr _root);
		void DrawBone( TSharedDebugDrawerPtr _debugDrawer, TSharedFrustrumPtr _frustrum, const Matrix44& _boneTransform );
		void DrawBoneLink( TSharedDebugDrawerPtr _debugDrawer, TSharedFrustrumPtr _frustrum, const Matrix44& _parentTransform, const Matrix44& _childTransform );
		float GetRadius(TSharedFrustrumPtr _frustrum, const Matrix44& _boneTransform, float _nearPlaneSize );
	};
}

#endif //__PANDUSkeletonDebugObject_h__