#include "PANDUDebugScene.h"

namespace Pandu
{
	DebugScene::DebugScene(TDebugDrawerAutoPtr _debugDrawer, TFrustrumWeakPtr _frustrumPtr)
		: m_DebugDrawer(_debugDrawer.release())
		, m_FrustrumPtr(_frustrumPtr)
	{
		m_DebugSceneMaterial.reset(new Material("DebugSceneMaterial"));
		m_DebugSceneMaterial->SetLightingEnabled(false);
	}

	DebugScene::~DebugScene()
	{
		Destroy();
	}

	void DebugScene::OnRenderFinished()
	{
		TTSharedDebugObjectPtrList::iterator itr = m_AllDebugObjects.begin();

		m_DebugSceneMaterial->EnableMaterialProperties();
		while( itr != m_AllDebugObjects.end() )
		{
			(*itr)->Render();
			if( (*itr)->CanDelete() )
			{
				PANDU_ERROR((*itr).use_count() == 1, "Debug object is still being used by another object, clear the reference before destroying");
				(*itr).reset();
				itr = m_AllDebugObjects.erase(itr);
				if( itr == m_AllDebugObjects.end() )
				{
					break;
				}
				else
				{
					continue;
				}
			}
			itr++;
		}
		m_DebugSceneMaterial->DisableMaterialProperties();
	}

	void DebugScene::AddDebugObject(TDebugObjectAutoPtr _debugObject)
	{
		TSharedDebugObjectPtr newDebugObject(_debugObject.release());
		m_AllDebugObjects.push_back( newDebugObject );
		newDebugObject->SetDebugDrawer(m_DebugDrawer);
		newDebugObject->SetFrustrum(m_FrustrumPtr);
	}

	void DebugScene::Destroy()
	{
		TTSharedDebugObjectPtrList::iterator itr = m_AllDebugObjects.begin();

		while( itr != m_AllDebugObjects.end() )
		{
			PANDU_ERROR((*itr).use_count() == 1, "Debug object is still being used by another object, clear the reference before destroying");
			(*itr).reset();
			itr++;
		}

		m_AllDebugObjects.clear();

		PANDU_ERROR(m_DebugDrawer.use_count() == 1, "Debug drawer instance is being used by another object, clear the reference before destroying");
		m_DebugDrawer.reset();
	}
}