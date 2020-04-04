#include "PANDUStringUtils.h"

namespace Pandu
{
	String StringUtils::GetLastString(const String& _string, char _seperator)
	{
		StringTokenizer tokenizer(_string,_seperator);

		String lastString;

		while(tokenizer.HasNextToken())
		{
			lastString = tokenizer.GetNextToken();
		}

		return lastString;
	}
}