#include "PANDURigidBodyToNodeTransformer.h"

namespace Pandu
{
	TBulletRigidBodySharedPtr ;
		TSharedNodePtr m_Node;

	RigidBodyToNodeTransformer::RigidBodyToNodeTransformer(TBulletRigidBodySharedPtr _rb,TSharedNodePtr _node)
		: m_RigidBody(_rb)
		, m_Node(_node)
	{
		BOOST_ASSERT( m_Node != NULL && m_RigidBody != NULL);
	}

	RigidBodyToNodeTransformer::~RigidBodyToNodeTransformer()
	{
		m_RigidBody.reset();
		m_Node.reset();
	}

	void RigidBodyToNodeTransformer::Update(const FrameEvent& _frameEvent)
	{
		const btTransform& transform = m_RigidBody->getWorldTransform();
		btQuaternion quat = transform.getRotation();
		m_Node->SetRotation(Quaternion(quat.w(),quat.x(),quat.y(),quat.z()));
		btVector3 pos = transform.getOrigin();
		m_Node->SetPosition(Vector3(pos.x(),pos.y(),pos.z()));
	}
}