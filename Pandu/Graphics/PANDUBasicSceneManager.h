/********************************************************************
	filename: 	PANDUBasicSceneManager
	author:		Parag Moni Boro
	
	purpose:	Game Engine created for learning
*********************************************************************/

#ifndef __PANDUBasicSceneManager_h__
#define __PANDUBasicSceneManager_h__

#include "PANDUSceneManager.h"
#include "PANDURenderable.h"

namespace Pandu
{
	class BasicSceneManager : public SceneManager
	{
	private:

		static const String m_ClassName;
		
		TRenderQueue m_RenderQueue;
		TSceneLightList m_SceneLights;

	public:

		static const String& GetClassType()	{	return m_ClassName;		}
		static void RegisterTravelClass();
	
		BasicSceneManager(const String& _name);
		virtual ~BasicSceneManager();

	protected:

		void ResetRenderQueue();
		void ResetLightList();

		void ApplySceneLights();
		void ApplySceneLight(const Matrix44& _mat,Light* _light);

		virtual void UpdateScene();
		virtual void RenderScene(unsigned int _techniqueIdToRenderScene);

	private:

		void UpdateSceneHierarchy(SceneNode* _parent);
		void AddItemsToRenderQueueFromSceneNode(SceneNode* _node);
		void RenderInternal(const Matrix44& _mat,Renderable* _renderable, unsigned int _techniqueIdToRenderScene);
		virtual void PreDestroy(){}
		virtual void PostDestroy(){}
	};
}

#endif