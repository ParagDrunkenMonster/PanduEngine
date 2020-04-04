#include "PANDUOverlaySceneManager.h"
#include "PANDUGraphicsRoot.h"
#include "PANDUGlInclude.h"

namespace
{
	static Pandu::TSharedSceneManagerPtr CreateOverlaySceneManager(const Pandu::String& _name)
	{
		Pandu::TSharedSceneManagerPtr scnMgr(new Pandu::OverlaySceneManager(_name));
		return scnMgr;
	}
}

namespace Pandu
{
	const String OverlaySceneManager::m_ClassName = String("OverlaySceneManager");

	OverlaySceneManager::OverlaySceneManager(const String& _name)
		: SceneManager(_name)
	{
		m_Flags.SetFlag(eSF_UseFixedPipelineToRenderIfNoTechnique);
	}

	OverlaySceneManager::~OverlaySceneManager()
	{
		ResetRenderQueue();
	}

	void OverlaySceneManager::PreDestroy()
	{
		
	}

	void OverlaySceneManager::PostDestroy()
	{
		ClearAllOverlays();
	}

	void OverlaySceneManager::RegisterTravelClass()
	{
		gGraphicsRoot->RegisterClassCreator(GetClassType(), &CreateOverlaySceneManager);
	}


	void OverlaySceneManager::UpdateScene()
	{
		TSharedSceneNodePtrList::iterator itr = m_SceneNodesList.begin();
		TSharedSceneNodePtrList::iterator end = m_SceneNodesList.end();

		for( ; itr != end ; itr++ )
		{
			(*itr)->Update();
        }

		// generate render queue
		ResetRenderQueue();
		GenerateRenderQueue();
	}

	void OverlaySceneManager::RenderScene(unsigned int _techniqueIdToRenderScene)
	{
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

	TOverlaySharedPtr OverlaySceneManager::AddOverlay(TOverlayAutoPtr _overlay)
	{
		TOverlaySharedPtr strongOverlay(_overlay.release());
		m_AllOverlays.push_back(strongOverlay);

		return strongOverlay;
	}

	void OverlaySceneManager::RemoveOverlay(TOverlayAutoPtr _overlay)
	{
		TOverlayPtrList::iterator itr = m_AllOverlays.begin();
		TOverlayPtrList::iterator end = m_AllOverlays.end();

		for( ; itr != end ; itr++ )
		{
			if(_overlay.get() == (*itr).get())
			{
				_overlay.release();
				PANDU_ERROR((*itr).use_count() == 1, (String("Overlay named ") + (*itr)->GetName()
						+ " is still in use, first clear the reference before clear.." ).CString() );

				(*itr).reset();

				itr = m_AllOverlays.erase(itr);
				break;
			}
		}
	}

	void OverlaySceneManager::ClearAllOverlays()
	{
		TOverlayPtrList::iterator itr = m_AllOverlays.begin();
		TOverlayPtrList::iterator end = m_AllOverlays.end();

		for( ; itr != end ; itr++ )
		{
			PANDU_ERROR((*itr).use_count() == 1, (String("Overlay named ") + (*itr)->GetName()
					+ " is still in use, first clear the reference before clear.." ).CString() );

			(*itr).reset();
			break;
		}

		m_AllOverlays.clear();
	}

	void OverlaySceneManager::ResetRenderQueue()
	{
		m_RenderQueue.clear();
	}
		
	void OverlaySceneManager::GenerateRenderQueue()
	{
		TSharedSceneNodePtrList::iterator itr = m_SceneNodesList.begin();
		TSharedSceneNodePtrList::iterator end = m_SceneNodesList.end();

		TSharedSceneNodePtr found;

		for( ; itr != end ; itr++ )
		{
			found = (*itr);
			const TRenderablesList& currList = found->GetRenderablesList();
			TRenderablesList::const_iterator renItr = currList.begin();
			TRenderablesList::const_iterator renEnd = currList.end();

			for( ; renItr != renEnd ; renItr++ )
			{
				m_RenderQueue.push_back(TRenderQueueItem(found.get(),(*renItr).get()));
			}
		}
	}

	void OverlaySceneManager::RenderInternal(const Matrix44& _mat,Renderable* _renderable, unsigned int _techniqueIdToRenderScene)
	{
		_renderable->Render(_mat,_techniqueIdToRenderScene,CheckFlag(eSF_UseFixedPipelineToRenderIfNoTechnique));
	}
}