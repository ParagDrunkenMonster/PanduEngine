#include "PANDUSkeletonManager.h"

namespace Pandu
{
	//===============================================================================
	//SkeletonManager Implementation

	class SkeletonManager::Impl
	{
	private:

		TSharedSkeletonMap m_SkeletonMap;

	public:

		Impl()
		{
		}

		~Impl()
		{
			ClearAllSkeleton();
		}

		void ClearAllSkeleton()
		{
			m_SkeletonMap.clear();

			TSharedSkeletonMap::iterator itr = m_SkeletonMap.begin();
			TSharedSkeletonMap::iterator end = m_SkeletonMap.end();

			for( ; itr != end ; itr++ )
			{
				PANDU_ERROR((*itr).second.use_count() == 1, (String("Skeleton named ") + (*itr).first
					+ " is still in use, first clear the reference before clear.." ).CString() );

				(*itr).second.reset();
			}

			m_SkeletonMap.clear();
		}

		TSharedSkeletonPtr CreateSkeleton(const String& _skeletonName)
		{
			TSharedSkeletonPtr skeletonPtr = GetSkeleton(_skeletonName);

			if(skeletonPtr != NULL)
			{
				return skeletonPtr;
			}

			TSharedSkeletonPtr newSkeleton(new Skeleton(_skeletonName));
			m_SkeletonMap.insert(std::pair<const String,TSharedSkeletonPtr>(newSkeleton->GetName(),newSkeleton));

			return newSkeleton;
		}

		TSharedSkeletonPtr GetSkeleton(const String& _skeletonName)
		{
			TSharedSkeletonMap::iterator itr = m_SkeletonMap.find(_skeletonName);

			if( itr == m_SkeletonMap.end() )
			{
				return TSharedSkeletonPtr();
			}
	
			return (*itr).second;
		}

		void DestroySkeleton(const String& _skeletonName)
		{
			TSharedSkeletonMap::iterator itr = m_SkeletonMap.find(_skeletonName);

			if( itr != m_SkeletonMap.end() )
			{
				PANDU_ERROR((*itr).second.use_count() == 1, (String("Skeleton named ") + _skeletonName
					+ " is still in use, first clear the reference before destroying.." ).CString() );
				(*itr).second.reset();
	
				itr = m_SkeletonMap.erase(itr);
			}
		}
	};

	//===============================================================================

	SkeletonManager* SkeletonManager::m_Instance = NULL;

	//--------------------------------------------------------------------
	//Static functions
	void SkeletonManager::CreateInstance()
	{
		if(m_Instance == NULL)
		{
			m_Instance = new SkeletonManager();
		}
	}

	void SkeletonManager::DestroyInstance()
	{
		if(m_Instance != NULL)
		{
			delete m_Instance;
			m_Instance = NULL;
		}
	}

	SkeletonManager* SkeletonManager::Get()
	{
		return m_Instance;
	}

	//--------------------------------------------------------------------

	SkeletonManager::SkeletonManager()
		: m_Impl(new Impl())
	{
	}

	SkeletonManager::~SkeletonManager()
	{
	}

	TSharedSkeletonPtr SkeletonManager::CreateSkeleton(const String& _skeletonName)
	{
		return m_Impl->CreateSkeleton(_skeletonName);
	}

	TSharedSkeletonPtr SkeletonManager::GetSkeleton(const String& _skeletonName)
	{
		return m_Impl->GetSkeleton(_skeletonName);
	}

	void SkeletonManager::DestroySkeleton(const String& _skeletonName)
	{
		m_Impl->DestroySkeleton(_skeletonName);
	}
}