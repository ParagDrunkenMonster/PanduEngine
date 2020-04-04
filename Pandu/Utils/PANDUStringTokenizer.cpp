#include "PANDUStringTokenizer.h"
#include "../boost/tokenizer.hpp"

namespace Pandu
{
	//===============================================================================
	//StringTokenizr Implementation
	class StringTokenizer::Impl
	{
	public:

		typedef boost::char_separator<char> TCharSeperator;
		typedef boost::tokenizer<TCharSeperator> TCharTokenizer;
		typedef TCharTokenizer::iterator TCharTokenizerItr;

		Impl(){}
		virtual ~Impl(){}

		virtual void ResetCursor() = 0;

		virtual String GetBegin() = 0;
		virtual String GetNextToken() = 0;
		virtual bool HasNextToken() const = 0;
	};

	//===============================================================================
	//StringTokenizr char separator Implementation
	class StringTokenizer::ImplCharSeparator : StringTokenizer::Impl
	{
	private:
		String					m_String;
		String					m_SeperatorValue;
		TCharSeperator			m_Seperator;
		TCharTokenizer			m_Tokenizer;
		TCharTokenizerItr		m_Itr;

	public:

		ImplCharSeparator(const String& _string, char _seperator)
			: Impl()
			, m_String(_string)
			, m_SeperatorValue(_seperator)
			, m_Seperator(m_SeperatorValue.CString())
			, m_Tokenizer(m_String.StdString(),m_Seperator) 
		{
			ResetCursor();
		}

		virtual ~ImplCharSeparator(){}

		virtual void ResetCursor()
		{
			m_Itr = m_Tokenizer.begin();
		}

		virtual String GetBegin()
		{
			TCharTokenizerItr beginItr = m_Tokenizer.begin();
			return String(*beginItr);
		}

		virtual String GetNextToken()
		{
			TCharTokenizerItr curr = m_Itr;

			if( m_Itr != m_Tokenizer.end() )
			{
				m_Itr++;
			}
			return String(*curr);
		}

		virtual bool HasNextToken() const
		{
			return (m_Itr != m_Tokenizer.end());
		}
	};

	//===============================================================================
	StringTokenizer::StringTokenizer(const String& _string,char _seperator)
		: m_Impl((Impl*)(new ImplCharSeparator(_string,_seperator)) )
	{
	}

	StringTokenizer::~StringTokenizer()
	{

	}

	void StringTokenizer::ResetCursor()
	{
		m_Impl->ResetCursor();
	}

	String StringTokenizer::GetBegin()
	{
		return m_Impl->GetBegin();
	}

	String StringTokenizer::GetNextToken()
	{
		return m_Impl->GetNextToken();
	}		

	bool StringTokenizer::HasNextToken() const
	{
		return m_Impl->HasNextToken();
	}
}