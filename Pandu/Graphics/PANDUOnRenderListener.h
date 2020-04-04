/********************************************************************
	filename: 	PANDUOnRenderListener
	author:		Parag Moni Boro
	
	purpose:	Game Engine created for learning
*********************************************************************/

#ifndef __PANDUOnRenderListener_h__
#define __PANDUOnRenderListener_h__

#include <boost/smart_ptr.hpp>
#include <list>
#include "PANDURenderable.h"

namespace Pandu
{
	class OnRenderListener;
	
	typedef boost::shared_ptr<OnRenderListener> TOnRenderListenerSharedPtr;
	typedef boost::weak_ptr<OnRenderListener> TOnRenderListenerWeakPtr;

	typedef std::list<TOnRenderListenerWeakPtr> TOnRenderListenerWeakPtrList;

	class OnRenderListener
	{
	public:
		OnRenderListener(){}
		virtual ~OnRenderListener(){}

		virtual void OnRender(TSharedRenderablePtr _renderable) = 0;
		virtual void OnRenderFinished() = 0;
	};
}

#endif