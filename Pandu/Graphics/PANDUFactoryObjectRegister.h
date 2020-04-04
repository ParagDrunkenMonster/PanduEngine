/********************************************************************
	filename: 	PANDUFactoryObjectRegister
	author:		Parag Moni Boro
	
	purpose:	Game Engine created for learning
*********************************************************************/

#ifndef __PANDUFactoryObjectRegister_h__
#define __PANDUFactoryObjectRegister_h__

#include <loki/Typelist.h>

namespace Pandu
{
	template <class TList, int idx>
	struct RegTravelClasses;

	template <class Head, class Tail>
	struct RegTravelClasses<Loki::Typelist<Head, Tail>, 0>
	{
		typedef Head Result;
		static inline void Fn(void)
		{
			Result::RegisterTravelClass();
		}
	};


	template <class Head, class Tail, int idx>
	struct RegTravelClasses<Loki::Typelist<Head, Tail>, idx>
	{
		typedef typename RegTravelClasses<Tail,idx - 1>::Result Result;
		static inline void Fn(void)
		{
			Result::RegisterTravelClass();
			RegTravelClasses<Loki::Typelist<Head, Tail>, idx - 1>::Fn();
		}
	};
}

#endif