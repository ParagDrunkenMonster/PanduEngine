/********************************************************************
	filename: 	PANDUCharacterUtils
	author:		Parag Moni Boro
	
	purpose:	Game Engine created for learning
*********************************************************************/

#ifndef __PANDUUtils_h__
#define __PANDUUtils_h__

#include "../boost/foreach.hpp"
#include "../boost/tokenizer.hpp"

namespace Pandu
{
	typedef boost::char_separator<char> TCharSeperator;
	typedef boost::tokenizer<TCharSeperator> TCharTokenizer;
	typedef TCharTokenizer::iterator TCharTokenizerItr;
}

#endif