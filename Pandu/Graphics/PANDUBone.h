/********************************************************************
	filename: 	PANDUBone
	author:		Parag Moni Boro
	
	purpose:	Game Engine created for learning
*********************************************************************/

#ifndef __PANDUBone_h__
#define __PANDUBone_h__

#include "../Utils/PANDUString.h"
#include "PANDUNode.h"
#include <vector>
#include <map>

namespace Pandu
{
	class Bone;

	typedef boost::shared_ptr<Bone> TSharedBonePtr;
	typedef boost::weak_ptr<Bone> TBoneWeakPtr;

	typedef std::vector<TSharedBonePtr> TBoneSharedPtrArray;
	typedef std::map<String,TSharedBonePtr> TBoneSharedPtrMap;

	class Bone : public Node
	{
	private:

		const unsigned int m_Index;

	public:
	
		Bone(const String& _name, unsigned int _index);
		virtual ~Bone();

		inline unsigned int GetIndex() const		{		return m_Index;				}

	private:

		virtual void UpdateFromParent(){}
		virtual void TransformsUpdated(){}
	};
}

#endif //__PANDUBone_h__