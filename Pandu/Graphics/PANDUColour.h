/********************************************************************
	filename: 	PANDUColour
	author:		Parag Moni Boro
	
	purpose:	Game Engine created for learning
*********************************************************************/

#ifndef __PANDUColour_h__
#define __PANDUColour_h__

namespace Pandu
{
	class Colour
	{
	public:
		union
		{
			struct
			{
				float r;
				float g;
				float b;
				float a;
			};

			float colourArray[4];
		};

	public:

		inline Colour()
		{
		}

		inline Colour( float _r, float _g, float _b, float _a)
			: r(_r)
			, g(_g)
			, b(_b)
			, a(_a)
		{
		}
	};
}

#endif