/********************************************************************
	filename: 	PANDUStringUtils
	author:		Parag Moni Boro
	
	purpose:	Game Engine created for learning
*********************************************************************/

#ifndef __PANDUStringUtils_h__
#define __PANDUStringUtils_h__

#include "PANDUString.h"
#include "PANDUStringTokenizer.h"

namespace Pandu
{
	class StringUtils
	{
	public:

		static String GetLastString(const String& _string, char _seperator);
	};
}

#endif 