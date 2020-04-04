/********************************************************************
	filename: 	PANDUOverlaySceneManager
	author:		Parag Moni Boro
	
	purpose:	Game Engine created for learning
*********************************************************************/

#ifndef __PANDUOverlaySceneManager_h__
#define __PANDUOverlaySceneManager_h__

#include "PANDUSceneManager.h"
#include "PANDUOverlayScreen.h"
#include <list>

namespace Pandu
{
	class OverlaySceneManager : public SceneManager
	{
	public:
		
		typedef std::list<TOverlaySharedPtr> TOverlayPtrList;

	private:

		static const String m_ClassName;

		TOverlayPtrList m_AllOverlays;
		TRenderQueue m_RenderQueue;

	public:

		static const String& GetClassType()	{	return m_ClassName;		}
		static void RegisterTravelClass();
	
		OverlaySceneManager(const String& _name);
		virtual ~OverlaySceneManager();

		TOverlaySharedPtr AddOverlay(TOverlayAutoPtr _overlay);
		void RemoveOverlay(TOverlayAutoPtr _overlay);

	protected:

		virtual void UpdateScene();
		virtual void RenderScene(unsigned int _techniqueIdToRenderScene);

		void ResetRenderQueue();

	private:
		
		void GenerateRenderQueue();
		void RenderInternal(const Matrix44& _mat,Renderable* _renderable, unsigned int _techniqueIdToRenderScene);

		void ClearAllOverlays();
		virtual void PreDestroy();
		virtual void PostDestroy();

	};
}

#endif