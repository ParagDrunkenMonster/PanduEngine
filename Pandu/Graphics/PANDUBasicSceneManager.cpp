#include "PANDUBasicSceneManager.h"
#include "PANDUGraphicsRoot.h"
#include "PANDUGlInclude.h"

namespace
{
	static Pandu::TSharedSceneManagerPtr CreateBasicSceneManager(const Pandu::String& _name)
	{
		Pandu::TSharedSceneManagerPtr scnMgr(new Pandu::BasicSceneManager(_name));
		return scnMgr;
	}
}

namespace Pandu
{
	const String BasicSceneManager::m_ClassName = String("BasicSceneManager");

	BasicSceneManager::BasicSceneManager(const String& _name)
		: SceneManager(_name)
	{
		m_Flags.SetFlag(eSF_UseFixedPipelineToRenderIfNoTechnique);
	}

	BasicSceneManager::~BasicSceneManager()
	{
		ResetRenderQueue();
		ResetLightList();
	}

	void BasicSceneManager::RegisterTravelClass()
	{
		gGraphicsRoot->RegisterClassCreator(GetClassType(), &CreateBasicSceneManager);
	}

	void BasicSceneManager::ResetRenderQueue()
	{
		m_RenderQueue.clear();
	}

	void BasicSceneManager::ResetLightList()
	{
		m_SceneLights.clear();
	}

	void BasicSceneManager::UpdateScene()
	{
		ResetRenderQueue();
		ResetLightList();
		UpdateSceneHierarchy(m_RootSceneNode.get());
	}

	void BasicSceneManager::RenderScene(unsigned int _techniqueIdToRenderScene)
	{
		ApplySceneLights();

		TRenderQueue::iterator itr = m_RenderQueue.begin();
		TRenderQueue::iterator end = m_RenderQueue.end();

		for( ; itr != end ; itr++ )
		{
			if( (*itr).first->GetVisible() )
			{
				const Matrix44& _transform = (*itr).first->GetWorldTransform();
				RenderInternal(_transform,(*itr).second,_techniqueIdToRenderScene);
			}
		}
	}

	void BasicSceneManager::RenderInternal(const Matrix44& _mat,Renderable* _renderable, unsigned int _techniqueIdToRenderScene)
	{
		_renderable->Render(_mat,_techniqueIdToRenderScene,CheckFlag(eSF_UseFixedPipelineToRenderIfNoTechnique));
	}

	//proper algorithms has to be implemented for generating render queue
	void BasicSceneManager::UpdateSceneHierarchy(SceneNode* _parent)
	{
		if( _parent->GetVisible() )
		{
			_parent->Update();
			if( _parent->IsNodeOfType(Node::eNT_SceneNode) )
			{
				AddItemsToRenderQueueFromSceneNode(_parent);
			}

			const TWeakNodePtrList& childsList = _parent->GetChildList();
			TWeakNodePtrList::const_iterator itr = childsList.begin();

			while( itr != childsList.end() )
			{
				TSharedNodePtr child = (*itr).lock();
				UpdateSceneHierarchy(static_cast<SceneNode*>(child.get()) );
			
				itr++;
			}
		}
	}

	void BasicSceneManager::AddItemsToRenderQueueFromSceneNode(SceneNode* _node)
	{
		//need to sort the queue based on bounding boxes.... 
		//will do later when descent scene will come to game
		const TRenderablesList& currList = _node->GetRenderablesList();
		TRenderablesList::const_iterator renItr = currList.begin();
		TRenderablesList::const_iterator renEnd = currList.end();

		for( ; renItr != renEnd ; renItr++ )
		{
			m_RenderQueue.push_back(TRenderQueueItem(_node,(*renItr).get()));
		}

		const TLightList& currLights = _node->GetLightList();
		TLightList::const_iterator ltItr = currLights.begin();
		TLightList::const_iterator ltEnd = currLights.end();

		for( ; ltItr != ltEnd ; ltItr++ )
		{
			m_SceneLights.push_back(TSceneLight(_node,(*ltItr).get()));
		}
	}

	void BasicSceneManager::ApplySceneLights()
	{
		const int count = (int)m_SceneLights.size();
		//PANDU_ERROR(count,"No light");
		if( count > 0 )
		{
			TSceneLightList::iterator itr = m_SceneLights.begin();
			TSceneLightList::iterator end = m_SceneLights.end();

			//for( ; itr != end ; itr++ )
			{
				if( (*itr).first->GetVisible() )
				{
					const Matrix44& _transform = (*itr).first->GetWorldTransform();
					ApplySceneLight(_transform,(*itr).second);
				}
			}
		}
	}

	void BasicSceneManager::ApplySceneLight(const Matrix44& _mat,Light* _light)
	{
		_light->ApplyLight(_mat,GL_LIGHT0);	
	}
}