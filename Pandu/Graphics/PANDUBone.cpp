#include "PANDUBone.h"

namespace Pandu
{
	Bone::Bone(const String& _name, unsigned int _index)
		: Node(eNT_Bone,_name)
		, m_Index(_index)
	{

	}

	Bone::~Bone()
	{

	}

}