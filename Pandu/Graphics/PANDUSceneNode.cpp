#include "PANDUSceneNode.h"

namespace Pandu
{
	//===============================================================================
	SceneNode::SceneNode(const String& _nodeName, bool _overrideParentTransforms)
		: Node(eNT_SceneNode,_nodeName,_overrideParentTransforms)
		, m_Visible(true)
	{
	}
	
	SceneNode::~SceneNode()
	{
		ClearAllRenderables();
		ClearAllLights();
	}

	void SceneNode::AddRenderable(TSharedRenderablePtr _renderable)
	{
		m_AllRenderables.push_back(_renderable);
	}

	void SceneNode::AddLight(TSharedLightPtr _light)
	{
		m_AllLights.push_back(_light);
	}

	void SceneNode::UpdateFromParent()
	{
	}

	void SceneNode::TransformsUpdated()
	{
	}

	void SceneNode::ClearAllRenderables()
	{
		TRenderablesList::iterator itr = m_AllRenderables.begin();
		TRenderablesList::iterator end = m_AllRenderables.end();

		for( ; itr != end ; itr++)
		{
			(*itr).reset();
		}
		m_AllRenderables.clear();
	}

	void SceneNode::ClearAllLights()
	{
		TLightList::iterator itr = m_AllLights.begin();
		TLightList::iterator end = m_AllLights.end();

		for( ; itr != end ; itr++)
		{
			(*itr).reset();
		}
		m_AllLights.clear();
	}
}