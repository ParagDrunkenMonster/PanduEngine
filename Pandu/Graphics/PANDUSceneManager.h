/********************************************************************
	filename: 	PANDUSceneManager
	author:		Parag Moni Boro
	
	purpose:	Game Engine created for learning
*********************************************************************/

#ifndef __PANDUSceneManager_h__
#define __PANDUSceneManager_h__

#include "../boost/smart_ptr.hpp"
#include "PANDUSceneNode.h"
#include "Utils/PANDUFlag.h"
#include "../Utils/PANDUCoreDefines.h"
#include <list>

namespace Pandu
{
	class SceneManager;
	typedef boost::shared_ptr<SceneManager> TSharedSceneManagerPtr;

	class SceneManager
	{
	public:

		typedef std::pair<SceneNode*,Renderable*> TRenderQueueItem;
		typedef std::list<TRenderQueueItem> TRenderQueue;
		
		typedef std::list<TSharedSceneNodePtr> TSharedSceneNodePtrList;

		typedef std::pair<SceneNode*,Light*> TSceneLight;
		typedef std::list<TSceneLight> TSceneLightList;

		enum SceneFlags
		{
			eSF_None									= 0,
			eSF_UseFixedPipelineToRenderIfNoTechnique	= 1 << 0,
		};

	protected:

		const String m_Name;

		TSharedSceneNodePtrList m_SceneNodesList;
		TSharedSceneNodePtr m_RootSceneNode;

		Flag<unsigned int> m_Flags;

	public:

		SceneManager(const String& _name);
		virtual ~SceneManager();

		inline TSharedSceneNodePtr GetRootSceneNode()	const {		return m_RootSceneNode;		}

		TSharedSceneNodePtr CreateSceneNode(const String& _name);
		TSharedSceneNodePtr GetSceneNode(const String& _name);
		
		TSharedSceneNodePtr CreateChildSceneNodeOfNode(TSharedSceneNodePtr _parent, const String& _childName);

		void DestroySceneNode(const String& _name);
		void ClearAllSceneNodes();

		inline const String& GetName() const		{	return m_Name;	}
		virtual void UpdateScene() = 0;
		virtual void RenderScene(unsigned int _techniqueIdToRenderScene) = 0;

		inline void SetFlag(SceneFlags _flag)		{	m_Flags.SetFlag(_flag);				}
		inline bool CheckFlag(SceneFlags _flag)		{	return m_Flags.CheckFlag(_flag);	}

	private:

		virtual void PreDestroy(){};
		virtual void PostDestroy(){};
	};
}

#endif