/********************************************************************
	filename: 	PANDUDebugObject
	author:		Parag Moni Boro
	
	purpose:	Game Engine created for learning
*********************************************************************/

#ifndef __PANDUDebugObject_h__
#define __PANDUDebugObject_h__

#include "../boost/smart_ptr.hpp"
#include <list>
#include "PANDUDebugDrawer.h"
#include "../Graphics/PANDUFrustrum.h"

namespace Pandu
{
	class DebugObject;

	typedef boost::shared_ptr<DebugObject> TSharedDebugObjectPtr;
	typedef boost::weak_ptr<DebugObject> TDebugObjectWeakPtr;
	typedef std::auto_ptr<DebugObject> TDebugObjectAutoPtr;

	typedef std::list<TSharedDebugObjectPtr> TTSharedDebugObjectPtrList;

	class DebugObject
	{
	protected:

		friend class DebugScene;

		TDebugDrawerWeakPtr m_DebugDrawer;
		TFrustrumWeakPtr m_FrustrumPtr;

	public:

		DebugObject(){}
		virtual ~DebugObject(){}

		virtual void Render() = 0;

	private:

		inline void SetDebugDrawer(TDebugDrawerWeakPtr _debugDrawer)
		{
			m_DebugDrawer = _debugDrawer;
			this->DebugDrawerUpdated();
		}

		virtual void DebugDrawerUpdated() = 0;
		virtual bool CanDelete() const = 0;

		inline void SetFrustrum(TFrustrumWeakPtr _frustrumPtr)
		{
			m_FrustrumPtr = _frustrumPtr;
		}
	};
}

#endif


