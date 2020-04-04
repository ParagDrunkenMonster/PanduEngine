/********************************************************************
	filename: 	PANDUDebugScene
	author:		Parag Moni Boro
	
	purpose:	Game Engine created for learning
*********************************************************************/

#ifndef __PANDUDebugScene_h__
#define __PANDUDebugScene_h__

#include "../Graphics/PANDUOnRenderListener.h"
#include "PANDUDebugObject.h"
#include "../Graphics/PANDUMaterial.h"

namespace Pandu
{
	class DebugScene;

	typedef boost::shared_ptr<DebugScene> TSharedDebugScenePtr;
	typedef boost::weak_ptr<DebugScene> TDebugSceneWeakPtr;

	class DebugScene : public Pandu::OnRenderListener
	{
	protected:

		
		TSharedDebugDrawerPtr m_DebugDrawer;
		TTSharedDebugObjectPtrList m_AllDebugObjects;

		TFrustrumWeakPtr m_FrustrumPtr;

		TSharedMaterialPtr m_DebugSceneMaterial;
		

	public:

		DebugScene(TDebugDrawerAutoPtr _debugDrawer, TFrustrumWeakPtr _frustrumPtr);
		virtual ~DebugScene();

		virtual void OnRender(TSharedRenderablePtr){}
		virtual void OnRenderFinished();

		void AddDebugObject(TDebugObjectAutoPtr _debugObject);

	protected:

		void Destroy();
	};
}

#endif