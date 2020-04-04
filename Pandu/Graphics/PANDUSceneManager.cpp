#include "PANDUSceneManager.h"

namespace Pandu
{
	SceneManager::SceneManager(const String& _name)
			: m_Name(_name)
			, m_RootSceneNode(CreateSceneNode("RootSceneNode"))
	{
	}

	SceneManager::~SceneManager()
	{
		PreDestroy();
		ClearAllSceneNodes();
		PostDestroy();
	}

	TSharedSceneNodePtr SceneManager::CreateSceneNode(const String& _name)
	{
		TSharedSceneNodePtr node = GetSceneNode(_name);

		PANDU_ERROR(node == NULL , ("SceneNode named " + node->GetName() 
			+ " already exists" ).CString() );

		if(node == NULL)
		{
			node.reset(new SceneNode(_name));

			m_SceneNodesList.push_back(node);
		}

		return node;
	}

	TSharedSceneNodePtr SceneManager::GetSceneNode(const String& _name)
	{
		TSharedSceneNodePtrList::iterator itr = m_SceneNodesList.begin();
		TSharedSceneNodePtrList::iterator end = m_SceneNodesList.end();

		TSharedSceneNodePtr found;

		for( ; itr != end ; itr++ )
		{
			if((*itr)->GetName() == _name )
			{
				found = (*itr);
				break;
			}
		}

		return found;
	}

	TSharedSceneNodePtr SceneManager::CreateChildSceneNodeOfNode(TSharedSceneNodePtr _parent, const String& _childName)
	{
		TSharedSceneNodePtr node = GetSceneNode(_childName);

		PANDU_ERROR(node == NULL , ("SceneNode named " + node->GetName() + " already exists" ).CString() );
		if(node == NULL)
		{
			node.reset(new SceneNode(_childName));

			AddChildToNode(_parent,node);

			m_SceneNodesList.push_back(node);

			return node;
		}

		return TSharedSceneNodePtr();
	}

	void SceneManager::DestroySceneNode(const String& _name)
	{
		TSharedSceneNodePtrList::iterator itr = m_SceneNodesList.begin();
		TSharedSceneNodePtrList::iterator end = m_SceneNodesList.end();

		for( ; itr != end ; itr++ )
		{
			if((*itr)->GetName() == _name )
			{
				PANDU_ERROR((*itr).use_count() == 1 , (String("SceneNode named ") + (*itr)->GetName() 
					+ String(" could not be deleted still in use, but removed from list, may have memory leaks") ).CString() );

				TSharedSceneNodePtr parent = (*itr);
				if(parent != NULL)
				{
					RemoveChildFromeNode(parent,*itr);
				}
				(*itr).reset();
				itr = m_SceneNodesList.erase(itr);

				break;
			}
		}
	}

	void SceneManager::ClearAllSceneNodes()
	{
		m_RootSceneNode.reset();

		TSharedSceneNodePtrList::iterator itr = m_SceneNodesList.begin();
		TSharedSceneNodePtrList::iterator end = m_SceneNodesList.end();

		TSharedSceneNodePtr found;

		for( ; itr != end ; itr++ )
		{
			PANDU_ERROR((*itr).use_count() == 1 , (String("SceneNode named ") + (*itr)->GetName() 
				+ String(" could not be deleted still in use, but removed from list, may have memory leaks") ).CString() );
			(*itr).reset();
		}

		m_SceneNodesList.clear();
	}
}