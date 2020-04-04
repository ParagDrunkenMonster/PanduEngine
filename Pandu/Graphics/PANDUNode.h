/********************************************************************
	filename: 	PANDUNode
	author:		Parag Moni Boro
	
	purpose:	Game Engine created for learning
*********************************************************************/

#ifndef __PANDUNode_h__
#define __PANDUNode_h__

#include "../Math/PANDUMatrix44.h"
#include "../Math/PANDUVector3.h"
#include "../Math/PANDUQuaternion.h"
#include <list>
#include "../Utils/PANDUString.h"
#include "../boost/smart_ptr.hpp"
#include "../Utils/PANDUCoreDefines.h"

namespace Pandu
{
	class Node;

	typedef boost::shared_ptr<Node> TSharedNodePtr;
	typedef boost::weak_ptr<Node> TNodeWeakPtr;

	typedef std::list<TNodeWeakPtr> TWeakNodePtrList;

	class Node
	{
	public:

		//it will be better to implement as typelist, will do in future ;)
		enum NodeType
		{
			eNT_SceneNode,
			eNT_Bone,
		};

	protected:

		const NodeType m_NodeType;
		const String m_NodeName;

		TWeakNodePtrList m_ChildList;

		TNodeWeakPtr m_Parent;
	
		Vector3 m_PositionWRTParent;
		Quaternion m_RotationWRTParent;
		Matrix44	m_WorldTransformMatrix;
		Vector3 m_Offset;
		bool m_DirtyTransform;
		bool m_OverrideParentTransforms;

		Node(NodeType _nodeType, const String& _nodeName,bool _overrideParentTransforms = false);
		
	public:
	
		virtual ~Node();


		const String& GetName() const;
		void ClearAllChilds();
		void ResetParent();

		const Vector3& GetPosition() const				{	return m_PositionWRTParent;		}
		const Quaternion& GetRotation() const			{	return m_RotationWRTParent;		}
			
		void SetRotation(const Quaternion& _rotation);
		void SetPosition(const Vector3& _trans);
		void SetOffset(const Vector3& _offset);
	
		const Matrix44& GetWorldTransform() const			{	return m_WorldTransformMatrix;	}
	
		void Update();

		inline void SetOverrideParentTransforms(bool _override)			{	m_OverrideParentTransforms = _override;	}
		inline TSharedNodePtr GetParent()								{	return m_Parent.lock();					}
		const TWeakNodePtrList& GetChildList() const					{	return m_ChildList;						}

		inline NodeType GetNodeType() const								{	return m_NodeType;						}
		inline bool IsNodeOfType(NodeType _nodeType) const				{	return m_NodeType == _nodeType;			}

		//----------------------------------------------------------------------------
		//friend functions

		friend void AddChildToNode(TSharedNodePtr _node,TSharedNodePtr _childToAdd);
		friend void RemoveChildFromeNode(TSharedNodePtr _node,TSharedNodePtr _childToRemove);
		
	private:

		virtual void UpdateFromParent() = 0;
		virtual void TransformsUpdated() = 0;

		void MakeAllChildTransformDirty();
		
		void SetTransformDirty()	{	m_DirtyTransform = true;	}

		void UpdateTransformMatrix();
	};
}

#endif