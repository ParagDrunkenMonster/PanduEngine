#include "PANDUMaterialManager.h"

namespace Pandu
{
	//===============================================================================
	//MaterialManager Implementation

	class MaterialManager::Impl
	{
	private:

		TSharedMaterialPtrList m_MaterialList;		

	public:

		Impl()
		{
		}

		~Impl()
		{
			ClearAllMaterial();
		}

		void ClearAllMaterial()
		{
			TSharedMaterialPtrList::iterator itr = m_MaterialList.begin();
			TSharedMaterialPtrList::iterator end = m_MaterialList.end();

			for( ; itr != end ; itr++ )
			{
				PANDU_ERROR((*itr).use_count() == 1, (String("Material named ") + (*itr)->GetName()
						+ " is still in use, first clear the reference before clear.." ).CString() );

				(*itr).reset();
			}

			m_MaterialList.clear();
		}

		TSharedMaterialPtr CreateMaterial(const String& _materialName)
		{
			TSharedMaterialPtr materialPtr = GetMaterial(_materialName);

			if(materialPtr != NULL)
			{
				return materialPtr;
			}

			TSharedMaterialPtr newMaterial(new Material(_materialName));
			m_MaterialList.push_back(newMaterial);

			return newMaterial;
		}

		TSharedMaterialPtr GetMaterial(const String& _materialName)
		{
			TSharedMaterialPtrList::iterator itr = m_MaterialList.begin();
			TSharedMaterialPtrList::iterator end = m_MaterialList.end();

			TSharedMaterialPtr materialFound;

			for( ; itr != end ; itr++ )
			{
				if((*itr)->GetName() == _materialName)
				{
					materialFound = *itr;
					break;
				}
			}

			return materialFound;
		}

		void DestroyMaterial(const String& _materialName)
		{
			TSharedMaterialPtrList::iterator itr = m_MaterialList.begin();
			TSharedMaterialPtrList::iterator end = m_MaterialList.end();

			for( ; itr != end ; itr++ )
			{
				if((*itr)->GetName() == _materialName)
				{
					PANDU_ERROR((*itr).use_count() == 1, (String("Material named ") + _materialName
							+ " is still in use, first clear the reference before destroying.." ).CString() );
					(*itr).reset();
					m_MaterialList.erase(itr);
					break;
				}
			}
		}
	};

	//===============================================================================

	MaterialManager* MaterialManager::m_Instance = NULL;

	//--------------------------------------------------------------------
	//Static functions
	void MaterialManager::CreateInstance()
	{
		if(m_Instance == NULL)
		{
			m_Instance = new MaterialManager();
		}
	}

	void MaterialManager::DestroyInstance()
	{
		if(m_Instance != NULL)
		{
			delete m_Instance;
			m_Instance = NULL;
		}
	}

	MaterialManager* MaterialManager::Get()
	{
		return m_Instance;
	}

	//--------------------------------------------------------------------

	MaterialManager::MaterialManager()
		: m_Impl(new Impl())
	{
	}
	
	MaterialManager::~MaterialManager()
	{
	}

	TSharedMaterialPtr MaterialManager::CreateMaterial(const String& _materialName)
	{
		return m_Impl->CreateMaterial(_materialName);
	}

	TSharedMaterialPtr MaterialManager::GetMaterial(const String& _materialName)
	{
		return m_Impl->GetMaterial(_materialName);
	}

	void MaterialManager::DestroyMaterial(const String& _materialName)
	{
		m_Impl->DestroyMaterial(_materialName);
	}
}