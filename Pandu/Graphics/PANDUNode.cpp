#include "PANDUNode.h"

namespace Pandu
{
	//===============================================================================
	Node::Node(NodeType _nodeType, const String& _nodeName,bool _overrideParentTransforms)
		: m_NodeType(_nodeType)
		, m_NodeName(_nodeName)
		, m_PositionWRTParent(Vector3::ZERO)
		, m_RotationWRTParent(Quaternion::IDENTITY)
		, m_WorldTransformMatrix(Matrix44::IDENTITY)
		, m_Offset(Vector3::ZERO)
		, m_DirtyTransform(true)
		, m_OverrideParentTransforms(_overrideParentTransforms)
	{

	}
	
	Node::~Node()
	{
		ClearAllChilds();
		ResetParent();
	}

	const String& Node::GetName() const
	{
		return m_NodeName;
	}
	
	void Node::SetRotation(const Quaternion& _rotation)
	{
		if(_rotation != m_RotationWRTParent )
		{
			m_RotationWRTParent = _rotation;
			SetTransformDirty();
			MakeAllChildTransformDirty();
		}
	}
	
	void Node::SetPosition(const Vector3& _trans)
	{
		if( _trans != m_PositionWRTParent )
		{
			m_PositionWRTParent = _trans;
			SetTransformDirty();
			MakeAllChildTransformDirty();
		}
	}
	
	void Node::SetOffset(const Vector3& _offset)
	{
		if( _offset != m_Offset )
		{
			m_Offset = _offset;
			SetTransformDirty();
			MakeAllChildTransformDirty();
		}
	}
	
	void Node::Update()
	{
		if(m_OverrideParentTransforms == false)
		{
			if(m_DirtyTransform == true)
			{
				TSharedNodePtr parent = m_Parent.lock();

				UpdateTransformMatrix();
				
				if(parent != NULL)
				{
					const Matrix44& parentTransform = parent->GetWorldTransform();
					m_WorldTransformMatrix = parentTransform * m_WorldTransformMatrix;
				}
						
				TransformsUpdated();
				m_DirtyTransform = false;
				MakeAllChildTransformDirty();
			}
		}
		else
		{
			if(m_DirtyTransform == true)
			{
				UpdateTransformMatrix();
			}
			m_DirtyTransform = false;
		}
		
		UpdateFromParent();
	}

	void Node::UpdateTransformMatrix()
	{
		Matrix44 localTransform;
		localTransform.MakeTransform(m_PositionWRTParent, m_RotationWRTParent);
		
		if( m_Offset != Vector3::ZERO )
		{
			Matrix44 offsetMat = Matrix44::IDENTITY;
			offsetMat.SetTranslate(m_Offset);
			localTransform = localTransform * offsetMat;
		}
		
		m_WorldTransformMatrix = localTransform;
	}

	void Node::MakeAllChildTransformDirty()
	{
		TWeakNodePtrList::iterator itr = m_ChildList.begin();
		TWeakNodePtrList::iterator end = m_ChildList.end();

		for( ; itr != end ; itr++ )
		{
			TSharedNodePtr currChild = (*itr).lock();
			if(currChild != NULL)
			{
				currChild->SetTransformDirty();
			}
			else
			{
				itr = m_ChildList.erase(itr);
				if(itr == end)
				{
					break;
				}
			}
		}
	}

	void Node::ResetParent()
	{
		m_Parent.reset();
	}

	void Node::ClearAllChilds()
	{
		TWeakNodePtrList::iterator itr = m_ChildList.begin();
		TWeakNodePtrList::iterator end = m_ChildList.end();

		for( ; itr != end ; itr++ )
		{
			(*itr).reset();
		}

		m_ChildList.clear();
	}

	void AddChildToNode(TSharedNodePtr _node,TSharedNodePtr _childToAdd)
	{
		TSharedNodePtr local = _childToAdd->m_Parent.lock();
		if(local != NULL)
		{
			gLogManager->WriteErrorLog(_childToAdd->GetName() + " already attached to node " + local->GetName() + ", remove it from this node to attach it to another node");
		}
		else
		{
			_node->m_ChildList.push_back(_childToAdd);	
			_childToAdd->m_Parent = _node;
			_childToAdd->MakeAllChildTransformDirty();
		}
	}
	
	void RemoveChildFromeNode(TSharedNodePtr _node,TSharedNodePtr _childToRemove)
	{
		TWeakNodePtrList::iterator itr = _node->m_ChildList.begin();
		TWeakNodePtrList::iterator end = _node->m_ChildList.end();

		for( ; itr != end ; itr++ )
		{
			TSharedNodePtr currChild = (*itr).lock();
			if( currChild == _childToRemove )
			{
				currChild->m_Parent.reset();// = NULL;
				currChild.reset();

				itr = _node->m_ChildList.erase(itr);
				
				_childToRemove->MakeAllChildTransformDirty();
				
				if(itr == end)
				{
					break;
				}
			}
			else
			{
				gLogManager->WriteErrorLog(_node->GetName() + " doesn't have a child " + _childToRemove->GetName() + " to remove");
			}
		}
	}
}