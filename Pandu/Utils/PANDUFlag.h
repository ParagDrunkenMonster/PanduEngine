/********************************************************************
	filename: 	PANDUFlag
	author:		Parag Moni Boro
	
	purpose:	Game Engine created for learning
*********************************************************************/

#ifndef __PANDUFlag_h__
#define __PANDUFlag_h__

namespace Pandu
{
	template <typename T> class Flag
	{
	private:

		T m_Flags;
	
	public:

		inline Flag() : m_Flags(0){}
		inline Flag(T _flag) : m_Flags(_flag){}
		inline Flag(const Flag& _flag) : m_Flags(_flag.m_Flags){}
		inline ~Flag(){}

		inline void SetFlag( T _flag )				{			m_Flags = m_Flags | _flag;					}
		inline void ClearFlag( T _flag )			{			m_Flags = m_Flags & ~_flag;					}
		inline bool CheckFlag( T _flag )	const	{			return ((m_Flags & _flag) == _flag);		}
		inline void ResetFlag()						{			m_Flags = 0;								}
	};

	typedef Flag<unsigned int> TFlag32;
}

#endif //__PANDUFlag_h__