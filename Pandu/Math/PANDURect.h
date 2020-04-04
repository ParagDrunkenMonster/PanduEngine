/********************************************************************
	filename: 	PANDURect
	author:		Parag Moni Boro
	
	purpose:	Game Engine created for learning
*********************************************************************/

#ifndef __PANDURect_h__
#define __PANDURect_h__

namespace Pandu
{
	class Rect
	{
	public:

		float m_Left;
		float m_Bottom;
		float m_Width;
		float m_Height;

		static const Rect gFullRect;

		Rect(float _left,float _bottom , float _width, float _height)
		{
			Set(_left,_bottom,_width,_height);
		}

		inline void Set(float _left,float _bottom , float _width, float _height)
		{
			m_Left = _left;
			m_Bottom = _bottom;
			m_Width = _width;
			m_Height = _height;
		}
	};
}

#endif