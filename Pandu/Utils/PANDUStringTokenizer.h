/********************************************************************
	filename: 	PANDUStringTokenizer
	author:		Parag Moni Boro
	
	purpose:	Game Engine created for learning
*********************************************************************/

#ifndef __PANDUStringTokenizer_h__
#define __PANDUStringTokenizer_h__

#include <boost/smart_ptr.hpp>
#include "PANDUString.h"

namespace Pandu
{
	class StringTokenizer
	{
	private:

		class Impl;
		class ImplCharSeparator;
		boost::scoped_ptr<Impl> m_Impl;

	public:
	
		StringTokenizer(const String& _string, char _seperator);
		~StringTokenizer();

		void ResetCursor();

		String GetBegin();
		String GetNextToken();
		bool HasNextToken() const;
	};
}

#endif //__PANDUStringTokenizer_h__