/********************************************************************
	filename: 	PANDUCommandParameter
	author:		Parag Moni Boro
	
	purpose:	Game Engine created for learning
*********************************************************************/

#ifndef __PANDUCommandParameter_h__
#define __PANDUCommandParameter_h__

namespace Pandu
{
	enum CommandParameter
	{
		eCP_None = 0,
		eCP_Help,

		eCP_Count,
	};

	static const char* gCommandParameterString[eCP_Count] = {
																0,			//eCP_None
																"/h",			//eCP_Help
															};
}

#endif