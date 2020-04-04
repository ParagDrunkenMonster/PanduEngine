#include "PANDUGraphicsRoot.h"
#include "PANDUSceneManager.h"
#include <boost/assert.hpp>
#include "PANDUFactoryObjectRegister.h"
#include "PANDUBasicSceneManager.h"
#include "PANDUOverlaySceneManager.h"
#include "PANDUMeshManager.h"
#include "PANDUMaterialManager.h"
#include "PANDULightManager.h"
#include "PANDUSkeletonManager.h"
#include "PANDUTextureManager.h"
#include "PANDUViewport.h"

namespace Pandu
{
	//===============================================================================
	//GraphicsRoot Implementation

	class GraphicsRoot::Impl
	{
	public:	

		typedef Loki::TL::MakeTypelist<BasicSceneManager,OverlaySceneManager>::Result TSceneManagerTypes;
		typedef RegTravelClasses<TSceneManagerTypes, Loki::TL::Length<TSceneManagerTypes>::value - 1> RegSceneManagerClassStruct;
		typedef std::list<TSharedSceneManagerPtr> TSharedSceneManagerPtrList;

		typedef std::list<TSharedViewportPtr> TSharedViewportPtrList;

	private:

		TRegisteredSceneManagerCreators m_SceneManagerCreators;
		TSharedSceneManagerPtrList m_SceneManagerList;	

		TSharedViewportPtrList m_ViewportsList;
		
	public:

		Impl()
		{
		}

		~Impl()
		{
			ClearAllViewports();
			ClearAllSceneManager();
			UnRegisterAllClassCreators();
		}

		void ClearAllSceneManager()
		{
			TSharedSceneManagerPtrList::iterator itr = m_SceneManagerList.begin();
			TSharedSceneManagerPtrList::iterator end = m_SceneManagerList.end();

			for( ; itr != end ; itr++ )
			{
				PANDU_ERROR((*itr).use_count() == 1, (String("SceneManager named ") + (*itr)->GetName()
						+ " is still in use, first clear the reference before clear.." ).CString() );

				(*itr).reset();
			}

			m_SceneManagerList.clear();
		}

		void ClearAllViewports()
		{
			TSharedViewportPtrList::iterator itr = m_ViewportsList.begin();
			TSharedViewportPtrList::iterator end = m_ViewportsList.end();

			for( ; itr != end ; itr++ )
			{
				PANDU_ERROR((*itr).use_count() == 1, (String("Viewport named ") + (*itr)->GetName()
						+ " is still in use, first clear the reference before clear.." ).CString() );

				(*itr).reset();
			}

			m_ViewportsList.clear();
		}

		TSharedSceneManagerPtr GetSceneManager(const String& _managerName)
		{
			TSharedSceneManagerPtrList::iterator itr = m_SceneManagerList.begin();
			TSharedSceneManagerPtrList::iterator end = m_SceneManagerList.end();

			TSharedSceneManagerPtr managerFound;

			for( ; itr != end ; itr++ )
			{
				if( (*itr)->GetName() == _managerName)
				{
					managerFound = *itr;
					break;
				}
			}

			return managerFound;
		}

		void DestroySceneManager(const String& _managerName)
		{
			TSharedSceneManagerPtrList::iterator itr = m_SceneManagerList.begin();
			TSharedSceneManagerPtrList::iterator end = m_SceneManagerList.end();

			for( ; itr != end ; itr++ )
			{
				if((*itr)->GetName() == _managerName)
				{
					PANDU_ERROR((*itr).use_count() == 1, (String("SceneManager named ") + _managerName
							+ " is still in use, first clear the reference before destroying.." ).CString() );
					(*itr).reset();
					itr = m_SceneManagerList.erase(itr);
					break;
				}
			}
		}

		
		TSharedSceneManagerPtr CreateSceneManager(const String& _typeName,const String& _managerName)
		{
			TSharedSceneManagerPtr managerPtr = GetSceneManager(_managerName);

			PANDU_ERROR(managerPtr == NULL , (String("SceneManager named ") + _managerName
							+ " already exists" ).CString() );

			if(managerPtr == NULL)
			{
				SceneManagerCreatorFunc fun = (*m_SceneManagerCreators.find(_typeName)).second;

				//TSharedSceneManagerPtr newSceneManager(new SceneManager(_managerName));
				//m_SceneManagerList.push_back(newSceneManager);

				TSharedSceneManagerPtr mewSceneManager = fun(_managerName);

				m_SceneManagerList.push_back(mewSceneManager);

				return mewSceneManager;
			}
			return managerPtr;
		}

		TSharedViewportPtr GetViewport(const String& _viewportName)
		{
			TSharedViewportPtrList::iterator itr = m_ViewportsList.begin();
			TSharedViewportPtrList::iterator end = m_ViewportsList.end();

			TSharedViewportPtr viewportFound;

			for( ; itr != end ; itr++ )
			{
				if( (*itr)->GetName() == _viewportName)
				{
					viewportFound = *itr;
					break;
				}
			}

			return viewportFound;
		}

		void DestroyViewport(const String& _viewportName)
		{
			TSharedViewportPtrList::iterator itr = m_ViewportsList.begin();
			TSharedViewportPtrList::iterator end = m_ViewportsList.end();

			for( ; itr != end ; itr++ )
			{
				if((*itr)->GetName() == _viewportName)
				{
					PANDU_ERROR((*itr).use_count() == 1, (String("Viewport named ") + _viewportName
							+ " is still in use, first clear the reference before destroying.." ).CString() );
					(*itr).reset();
					itr = m_ViewportsList.erase(itr);
					break;
				}
			}
		}

		
		TSharedViewportPtr CreateViewport(const String& _name, unsigned int _left , unsigned int _bottom , unsigned int _width , unsigned int _height)
		{
			TSharedViewportPtr viewportPtr = GetViewport(_name);

			PANDU_ERROR(viewportPtr == NULL , (String("Viewport named ") + _name
							+ " already exists" ).CString() );

			if(viewportPtr == NULL)
			{
				TSharedViewportPtr newViewport(new Viewport(_name,_left,_bottom,_width,_height));

				m_ViewportsList.push_back(newViewport);

				return newViewport;
			}
			return viewportPtr;
		}

		void RegisterClassCreator(const String& _typeName, SceneManagerCreatorFunc _creatorFn)
		{
			m_SceneManagerCreators.insert(TRegisteredSceneManagerCreators::value_type(_typeName, _creatorFn));
		}

		void UnRegisterAllClassCreators()
		{
			m_SceneManagerCreators.clear();
		}

		void RenderOneFrame()
		{
			TSharedViewportPtrList::iterator itr = m_ViewportsList.begin();
			TSharedViewportPtrList::iterator end = m_ViewportsList.end();

			for( ; itr != end ; itr++ )
			{
				TSharedViewportPtr viewport = *itr;
				viewport->UpdateViewport();
				viewport->RenderScene();
			}
		}
	};

	//===============================================================================

	GraphicsRoot* GraphicsRoot::m_Instance = NULL;

	//--------------------------------------------------------------------
	//Static functions
	void GraphicsRoot::CreateInstance()
	{
		if(m_Instance == NULL)
		{
			m_Instance = new GraphicsRoot();
			GraphicsRoot::Impl::RegSceneManagerClassStruct::Fn();
		}
	}

	void GraphicsRoot::DestroyInstance()
	{
		if(m_Instance != NULL)
		{
			delete m_Instance;
			m_Instance = NULL;
		}
	}

	GraphicsRoot* GraphicsRoot::Get()
	{
		return m_Instance;
	}

	//--------------------------------------------------------------------

	GraphicsRoot::GraphicsRoot()
		: m_Impl(new Impl())
	{
		TextureManager::CreateInstance();
		MaterialManager::CreateInstance();
		SkeletonManager::CreateInstance();
		MeshManager::CreateInstance();
		LightManager::CreateInstance();
	}
	
	GraphicsRoot::~GraphicsRoot()
	{
		m_Impl.reset();
		LightManager::DestroyInstance();
		MeshManager::DestroyInstance();
		SkeletonManager::DestroyInstance();
		MaterialManager::DestroyInstance();
		TextureManager::DestroyInstance();
	}

	TSharedSceneManagerPtr GraphicsRoot::GetSceneManager(const String& _managerName)
	{
		return m_Impl->GetSceneManager(_managerName);
	}

	void GraphicsRoot::DestroySceneManager(const String& _managerName)
	{
		m_Impl->DestroySceneManager(_managerName);
	}

	TSharedSceneManagerPtr GraphicsRoot::CreateSceneManager(const String& _typeName,const String& _managerName)
	{
		return m_Impl->CreateSceneManager(_typeName,_managerName);
	}

	TSharedViewportPtr GraphicsRoot::GetViewport(const String& _viewportName)
	{
		return m_Impl->GetViewport(_viewportName);
	}

	void GraphicsRoot::DestroyViewport(const String& _viewportName)
	{
		m_Impl->DestroyViewport(_viewportName);
	}

	TSharedViewportPtr GraphicsRoot::CreateViewport(const String& _name, unsigned int _left , unsigned int _bottom , unsigned int _width , unsigned int _height)
	{
		return m_Impl->CreateViewport(_name,_left , _bottom , _width , _height);
	}

	void GraphicsRoot::RegisterClassCreator(const String& _typeName, SceneManagerCreatorFunc _creatorFn)
	{
		m_Impl->RegisterClassCreator(_typeName,_creatorFn);
	}

	void GraphicsRoot::RenderOneFrame()
	{
		m_Impl->RenderOneFrame();
	}
}