#include "PANDUMeshManager.h"

namespace Pandu
{
	//===============================================================================
	//MeshManager Implementation

	class MeshManager::Impl
	{
	private:

		TSharedMeshPtrList m_MeshList;		

	public:

		Impl()
		{
		}

		~Impl()
		{
			ClearAllMesh();
		}

		void ClearAllMesh()
		{
			TSharedMeshPtrList::iterator itr = m_MeshList.begin();
			TSharedMeshPtrList::iterator end = m_MeshList.end();

			for( ; itr != end ; itr++ )
			{
				PANDU_ERROR((*itr).use_count() == 1, (String("Mesh named ") + (*itr)->GetName()
						+ " is still in use, first clear the reference before clear.." ).CString() );

				(*itr).reset();
			}

			m_MeshList.clear();
		}

		TSharedMeshPtr CreateMesh(const String& _meshName)
		{
			TSharedMeshPtr meshPtr = GetMesh(_meshName);

			if(meshPtr != NULL)
			{
				return meshPtr;
			}

			TSharedMeshPtr newMesh(new Mesh(_meshName));
			m_MeshList.push_back(newMesh);

			return newMesh;
		}

		TSharedMeshPtr CreateMesh(MeshManager::TMeshCreator _meshCreator)
		{
			TSharedMeshPtr newMesh =_meshCreator();

			if( newMesh )
			{
				m_MeshList.push_back(newMesh);
			}

			return newMesh;
		}

		TSharedMeshPtr GetMesh(const String& _meshName)
		{
			TSharedMeshPtrList::iterator itr = m_MeshList.begin();
			TSharedMeshPtrList::iterator end = m_MeshList.end();

			TSharedMeshPtr meshFound;

			for( ; itr != end ; itr++ )
			{
				if((*itr)->GetName() == _meshName)
				{
					meshFound = *itr;
					break;
				}
			}

			return meshFound;
		}

		void DestroyMesh(const String& _meshName)
		{
			TSharedMeshPtrList::iterator itr = m_MeshList.begin();
			TSharedMeshPtrList::iterator end = m_MeshList.end();

			for( ; itr != end ; itr++ )
			{
				if((*itr)->GetName() == _meshName)
				{
					PANDU_ERROR((*itr).use_count() == 1, (String("Mesh named ") + _meshName
							+ " is still in use, first clear the reference before destroying.." ).CString() );
					(*itr).reset();
					break;
				}
			}
		}
	};

	//===============================================================================

	MeshManager* MeshManager::m_Instance = NULL;

	//--------------------------------------------------------------------
	//Static functions
	void MeshManager::CreateInstance()
	{
		if(m_Instance == NULL)
		{
			m_Instance = new MeshManager();
		}
	}

	void MeshManager::DestroyInstance()
	{
		if(m_Instance != NULL)
		{
			delete m_Instance;
			m_Instance = NULL;
		}
	}

	MeshManager* MeshManager::Get()
	{
		return m_Instance;
	}

	//--------------------------------------------------------------------

	MeshManager::MeshManager()
		: m_Impl(new Impl())
	{
	}
	
	MeshManager::~MeshManager()
	{
	}

	TSharedMeshPtr MeshManager::CreateMesh(const String& _meshName)
	{
		return m_Impl->CreateMesh(_meshName);
	}

	TSharedMeshPtr MeshManager::CreateMesh(TMeshCreator _meshCreator)
	{
		return m_Impl->CreateMesh(_meshCreator);
	}

	TSharedMeshPtr MeshManager::GetMesh(const String& _meshName)
	{
		return m_Impl->GetMesh(_meshName);
	}

	void MeshManager::DestroyMesh(const String& _meshName)
	{
		m_Impl->DestroyMesh(_meshName);
	}
}