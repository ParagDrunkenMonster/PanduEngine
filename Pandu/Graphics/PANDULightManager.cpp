#include "PANDULightManager.h"

namespace Pandu
{
	//===============================================================================
	//LightManager Implementation

	class LightManager::Impl
	{
	private:

		TSharedLightPtrList m_LightList;		

	public:

		Impl()
		{
		}

		~Impl()
		{
			ClearAllLight();
		}

		void ClearAllLight()
		{
			TSharedLightPtrList::iterator itr = m_LightList.begin();
			TSharedLightPtrList::iterator end = m_LightList.end();

			for( ; itr != end ; itr++ )
			{
				PANDU_ERROR((*itr).use_count() == 1, (String("Light named ") + (*itr)->GetName()
						+ " is still in use, first clear the reference before clear.." ).CString() );

				(*itr).reset();
			}

			m_LightList.clear();
		}

		TSharedLightPtr CreateLight(const String& _lightName)
		{
			TSharedLightPtr lightPtr = GetLight(_lightName);

			if(lightPtr != NULL)
			{
				return lightPtr;
			}

			TSharedLightPtr newLight(new Light(_lightName));
			m_LightList.push_back(newLight);

			return newLight;
		}

		TSharedLightPtr GetLight(const String& _lightName)
		{
			TSharedLightPtrList::iterator itr = m_LightList.begin();
			TSharedLightPtrList::iterator end = m_LightList.end();

			TSharedLightPtr lightFound;

			for( ; itr != end ; itr++ )
			{
				if((*itr)->GetName() == _lightName)
				{
					lightFound = *itr;
					break;
				}
			}

			return lightFound;
		}

		void DestroyLight(const String& _lightName)
		{
			TSharedLightPtrList::iterator itr = m_LightList.begin();
			TSharedLightPtrList::iterator end = m_LightList.end();

			for( ; itr != end ; itr++ )
			{
				if((*itr)->GetName() == _lightName)
				{
					PANDU_ERROR((*itr).use_count() == 1, (String("Light named ") + _lightName
							+ " is still in use, first clear the reference before destroying.." ).CString() );
					(*itr).reset();
					m_LightList.erase(itr);
					break;
				}
			}
		}
	};

	//===============================================================================

	LightManager* LightManager::m_Instance = NULL;

	//--------------------------------------------------------------------
	//Static functions
	void LightManager::CreateInstance()
	{
		if(m_Instance == NULL)
		{
			m_Instance = new LightManager();
		}
	}

	void LightManager::DestroyInstance()
	{
		if(m_Instance != NULL)
		{
			delete m_Instance;
			m_Instance = NULL;
		}
	}

	LightManager* LightManager::Get()
	{
		return m_Instance;
	}

	//--------------------------------------------------------------------

	LightManager::LightManager()
		: m_Impl(new Impl())
	{
	}
	
	LightManager::~LightManager()
	{
	}

	TSharedLightPtr LightManager::CreateLight(const String& _lightName)
	{
		return m_Impl->CreateLight(_lightName);
	}

	TSharedLightPtr LightManager::GetLight(const String& _lightName)
	{
		return m_Impl->GetLight(_lightName);
	}

	void LightManager::DestroyLight(const String& _lightName)
	{
		m_Impl->DestroyLight(_lightName);
	}
}