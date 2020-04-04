/********************************************************************
	filename: 	PANDUInputBinding
	author:		Parag Moni Boro
	
	purpose:	Game Engine created for learning
*********************************************************************/

#ifndef __PANDUInputBinding_h__
#define __PANDUInputBinding_h__

#include "../boost/smart_ptr.hpp"

namespace Pandu
{
	class InputBinding;

	typedef boost::shared_ptr<InputBinding> TInputBindingSharedPtr;
	typedef std::auto_ptr<InputBinding> TInputBindingAutoPtr;

	class InputBinding
	{
	public:
		InputBinding()
		{
		}

		virtual ~InputBinding()
		{
		}

		virtual int GetGameInputFromPhysicalKey(int _key) = 0;
		virtual int GetPhysicalKeyFromGameInput(int _gameInput) = 0;
		virtual float GetTimeForGameInput(int _gameInput) = 0;
	};
}

#endif