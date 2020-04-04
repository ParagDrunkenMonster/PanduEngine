#include "PANDUSkeleton.h"

namespace
{
	typedef std::vector<Pandu::Skeleton::BoneInfo*> TBoneInfoArray;
	typedef std::map<Pandu::String,const Pandu::Skeleton::BoneInfo*> TBoneInfoMap;//String key is name of the bone;
	typedef std::vector<unsigned int> TBoneIndexArray;
	typedef std::vector<TBoneIndexArray> TChildBoneArray;

	bool HasValueInArray( const TBoneIndexArray& _array, unsigned int _childIndex)
	{
		TBoneIndexArray::const_iterator found = std::find(_array.begin(),_array.end(),_childIndex);
		return (found != _array.end());
	}
}

namespace Pandu
{
	
	//===============================================================================
	//Skeleton Implementation
	class Skeleton::Impl
	{
	public:
		
		//===============================================================================
		//SkeletonInstanceImpl Implementation
		class SkeletonInstanceImpl : public SkeletonInstance
		{
		private:

			Skeleton* m_Skeleton;
			TSharedBonePtr m_RootBone;
			TBoneSharedPtrArray m_BonesArray;

		public:

			SkeletonInstanceImpl(Skeleton* _skeleton);
			~SkeletonInstanceImpl();
			void ConstructBoneStructure();
			TSharedBonePtr ConstructBoneStructure(unsigned int _parentBone);

			TBoneWeakPtr GetRootBone() const	{	return m_RootBone;			}

			TBoneWeakPtr GetBoneByIndex(unsigned int _index)
			{
				const unsigned int count = (unsigned int)m_BonesArray.size();
				PANDU_ERROR(_index < count && count > 0, "Bone index can't be greater and equal to count");
				if( _index >= count )
				{
					return TSharedBonePtr();
				}

				return m_BonesArray[_index];
			}

			void Update()
			{
				const unsigned int count = (unsigned int)m_BonesArray.size();
				PANDU_ERROR( count > 0, "Bone count zero");

				for( unsigned int i = 0 ; i < count ; i++ )
				{
					m_BonesArray[i]->Update();
				}
			}

			int GetChildCountOfBone(unsigned int _boneIndex) const
			{
				const unsigned int count = (unsigned int)m_BonesArray.size();
				PANDU_ERROR(_boneIndex < count, "Bone index can't be greater and equal to count");
				if( _boneIndex >= count )
				{
					return -1;
				}

				return (int)m_Skeleton->m_Impl->m_ChildBoneIdArray[_boneIndex].size();
			}

			int GetBoneIdOfChildOfBone(unsigned int _boneIndex, unsigned int _childIndex) const
			{
				const unsigned int count = (unsigned int)m_BonesArray.size();
				PANDU_ERROR(_boneIndex < count, "Bone index can't be greater and equal to count");
				if( _boneIndex >= count )
				{
					return -1;
				}

				unsigned int childCount = (unsigned int)m_Skeleton->m_Impl->m_ChildBoneIdArray[_boneIndex].size();

				PANDU_ERROR(_childIndex < childCount, "Bone doesn't have that many child");
				if( _childIndex >= childCount )
				{
					return -1;
				}

				return (int)(m_Skeleton->m_Impl->m_ChildBoneIdArray[_boneIndex])[_childIndex];
			}

			const TBoneSharedPtrArray& GetBonesSharedPtrArray() const
			{
				return m_BonesArray;
			}

			unsigned int BoneCount() const
			{
				return (unsigned int)m_BonesArray.size();
			}

			const TSharedSkeletalAnimationPtr GetAnimation(const String& _name) const
			{
				return m_Skeleton->m_Impl->GetAnimation(_name);
			}

			const String& SkeletonName() const
			{
				return m_Skeleton->GetName();
			}
		};

		typedef boost::shared_ptr<SkeletonInstanceImpl> TSharedSkeletonInstanceImplPtr;
		typedef std::list<TSharedSkeletonInstanceImplPtr> TSharedSkeletonInstanceImplPtrList;

		//===============================================================================

		const String m_Name;
		Skeleton* m_Skeleton;
		TBoneInfoArray m_BoneInfoArray;	//Fast index based search
		TChildBoneArray m_ChildBoneIdArray;
		TBoneInfoMap m_BoneMap;		//Fast name based search
		unsigned int m_RootBoneIndex;

		TSharedSkeletonInstanceImplPtrList m_SkeletonInstances;
		TNameSharedSkeletanAnimationMap m_AllAnimations;

		Impl(const String& _name, Skeleton* _skeleton)
			: m_Name(_name)
			, m_Skeleton(_skeleton)
			, m_RootBoneIndex(0)
		{
		}

		~Impl()
		{
			Destroy();
		}

		void Destroy()
		{
			const unsigned int boneCount = (unsigned int)m_BoneInfoArray.size();
			for( unsigned int i = 0 ; i < boneCount ; i++ )
			{
				delete m_BoneInfoArray[i];
			}
			m_BoneInfoArray.clear();
			m_BoneMap.clear();

			for( unsigned int i = 0 ; i < boneCount ; i++ )
			{
				m_ChildBoneIdArray[i].clear();
			}
			m_ChildBoneIdArray.clear();

			ClearSkeletonInstances();
			DestroyAllAnims();

			m_Skeleton = NULL;
		}

		void ClearSkeletonInstances()
		{
			TSharedSkeletonInstanceImplPtrList::iterator itr = m_SkeletonInstances.begin();
			TSharedSkeletonInstanceImplPtrList::iterator end = m_SkeletonInstances.end();

			for( ; itr != end ; itr++ )
			{
				PANDU_ERROR((*itr).use_count() == 1, (String("Skeleton named ") + m_Name
					+ " is still in use, first clear the reference before clear.." ).CString() );

				(*itr).reset();
			}

			m_SkeletonInstances.clear();
		}

		TSharedSkeletonInstancePtr CreateSkeletonInstance()
		{
			TSharedSkeletonInstanceImplPtr sklInstance(new SkeletonInstanceImpl(m_Skeleton));
			m_SkeletonInstances.push_back(sklInstance);
			return sklInstance;
		}

		TSharedSkeletalAnimationPtr CreateAnimation(const String& _animName, unsigned int _frameCount)
		{
			TSharedSkeletalAnimationPtr anim = GetAnimation(_animName);

			if( anim == NULL )
			{
				const unsigned int boneCount = (unsigned int)m_BoneInfoArray.size();
				anim.reset(new SkeletalAnimation(_animName,boneCount,_frameCount));
				m_AllAnimations.insert(std::pair<const String,TSharedSkeletalAnimationPtr>(_animName,anim) );
				return anim;
			}
			else
			{
				PANDU_ERROR(false,"Animation already existed");
				gLogManager->WriteErrorLog(String("Animation named '") + _animName + "' already existed");
			}
			return TSharedSkeletalAnimationPtr();
		}

		void DestroyAllAnims()
		{
			TNameSharedSkeletanAnimationMap::iterator itr = m_AllAnimations.begin();
			while( itr != m_AllAnimations.end() )
			{
				PANDU_ERROR((*itr).second.use_count() == 1, (String("Skeletal animation named '") + (*itr).first 
					+ "' from skeleton '" + m_Name 
					+ "' is still in use, first clear the reference before clear.." ).CString() );

				(*itr).second.reset();
				itr++;
			}

			m_AllAnimations.clear();
		}

		TSharedSkeletalAnimationPtr GetAnimation(const String& _name)
		{
			TNameSharedSkeletanAnimationMap::iterator itr = m_AllAnimations.find(_name);
			if( itr != m_AllAnimations.end() )
			{
				return (*itr).second;
			}

			return TSharedSkeletalAnimationPtr();
		}

		unsigned int GetBoneCount() const
		{
			return (unsigned int)m_BoneInfoArray.size();
		}
	};

	//===============================================================================
	//SkeletonInstanceImpl
	Skeleton::Impl::SkeletonInstanceImpl::SkeletonInstanceImpl(Skeleton* _skeleton)
		: SkeletonInstance()
		, m_Skeleton(_skeleton)
	{
		ConstructBoneStructure();
	}

	Skeleton::Impl::SkeletonInstanceImpl::~SkeletonInstanceImpl()
	{
		m_Skeleton = NULL;
	}

	void Skeleton::Impl::SkeletonInstanceImpl::ConstructBoneStructure()
	{
		m_BonesArray.resize((unsigned int)m_Skeleton->m_Impl->m_BoneInfoArray.size());
		m_RootBone = ConstructBoneStructure(m_Skeleton->m_Impl->m_RootBoneIndex);
	}

	TSharedBonePtr Skeleton::Impl::SkeletonInstanceImpl::ConstructBoneStructure(unsigned int _parentBone)
	{
		const BoneInfo* rootBoneInfo = m_Skeleton->m_Impl->m_BoneInfoArray[_parentBone];
		PANDU_ERROR( rootBoneInfo->m_BoneIndex == _parentBone, "both the indices has to be same" );

		TSharedBonePtr parentBone(new Bone(rootBoneInfo->m_Name,_parentBone));
		const Matrix44& boneMat = rootBoneInfo->m_BoneMatrix;
		const Vector3 translate = boneMat.GetTranslate();
		//very unoptimized...
		const Matrix33 rotMat(boneMat.GetColumnVec3(0),boneMat.GetColumnVec3(1),boneMat.GetColumnVec3(2));
		const Quaternion rot(rotMat);
		parentBone->SetPosition(translate);
		parentBone->SetRotation(rot);
		m_BonesArray[_parentBone] = parentBone;

		const TBoneIndexArray& childsArray = m_Skeleton->m_Impl->m_ChildBoneIdArray[_parentBone];

		const unsigned int childsCount = (unsigned int)childsArray.size();

		for( unsigned int i = 0 ; i < childsCount ; i++ )
		{
			TSharedBonePtr child = ConstructBoneStructure(childsArray[i]);
			AddChildToNode(parentBone,child);
		}

		return parentBone;
	}

	//===============================================================================
	//Skeleton
	Skeleton::Skeleton(const String& _name)
	{
		m_Impl.reset(new Impl(_name,this));
	}

	Skeleton::~Skeleton()
	{
		
	}

	int Skeleton::CreateBoneInfo(const String& _boneName, const Vector3& _bonePos, const Quaternion& _boneRotation /*= Quaternion::IDENTITY*/)
	{
		Matrix44 boneMatrix = Matrix44::IDENTITY;
		boneMatrix.MakeTransform(_bonePos,_boneRotation);
		return CreateBoneInfo(_boneName,boneMatrix);
	}

	int Skeleton::CreateBoneInfo(const String& _boneName, const Matrix44& _boneMatrix)
	{
		const BoneInfo* existedBone = GetBoneInfoByName(_boneName);
		if( existedBone )
		{
			PANDU_ERROR( false , _boneName + " : Bone name already exist" );
			gLogManager->WriteErrorLog("Bone named '" + _boneName + "' already exist in skeleton '" + m_Impl->m_Name);
			return -1;
		}

		const int index = (int)m_Impl->m_BoneInfoArray.size();
		
		BoneInfo* newBoneInfo = new BoneInfo();
		newBoneInfo->m_Name = _boneName;
		newBoneInfo->m_BoneIndex = index;
		newBoneInfo->m_BoneMatrix = _boneMatrix;
		
		m_Impl->m_BoneInfoArray.push_back(newBoneInfo);
		m_Impl->m_ChildBoneIdArray.push_back(TBoneIndexArray());
		m_Impl->m_BoneMap.insert(std::pair<const String,const BoneInfo* const>(newBoneInfo->m_Name,newBoneInfo));
		
		return index;
	}

	void Skeleton::AddChildToBone(unsigned int _boneIndex, unsigned int _childIndex)
	{
		unsigned int boneCount = (unsigned int)m_Impl->m_BoneInfoArray.size();
		assert( _boneIndex < (unsigned int)boneCount );
		if( _boneIndex >= (unsigned int)boneCount )
		{
			PANDU_ERROR( false , "Child doesn't exist" );
			gLogManager->WriteErrorLog(String("No bone exist indexed ") + _boneIndex + " in skeleton " + m_Impl->m_Name );
			return;
		}
		else if( _boneIndex == _childIndex )
		{
			PANDU_ERROR( false , "Child and bone indices are same" );
			gLogManager->WriteErrorLog(String("Child index and bone indices are same ") + _boneIndex );
			return;
		}

		for( unsigned int i = 0 ; i < boneCount ; i++ )
		{
			if( i != _childIndex && HasValueInArray(m_Impl->m_ChildBoneIdArray[i],_childIndex) )
			{
				if( _boneIndex == i )
				{
					PANDU_ERROR( false , "Child already exist in bone" );
					gLogManager->WriteWarningLog(String("Bone indexed ") + _boneIndex + " already contains child indexed " + _childIndex );
					return;
				}
				else
				{
					PANDU_ERROR( false , "Child already added to another bone" );
					gLogManager->WriteWarningLog(String("Bone indexed ") + _childIndex + " already has a parent indexed " + i );
					return;
				}
			}
		}

		//update the root bone
		if( m_Impl->m_RootBoneIndex == _childIndex )
		{
			m_Impl->m_RootBoneIndex = _boneIndex;
		}

		m_Impl->m_ChildBoneIdArray[_boneIndex].push_back(_childIndex);
	}

	const Skeleton::BoneInfo* Skeleton::GetBoneInfoByName(const String& _boneName) const
	{
		TBoneInfoMap::const_iterator bonePtr = m_Impl->m_BoneMap.find(_boneName);
		if( bonePtr != m_Impl->m_BoneMap.end())
		{
			return (*bonePtr).second;
		}
		return NULL;
	}

	TSharedSkeletonInstancePtr Skeleton::CreateSkeletonInstance() const
	{
		return m_Impl->CreateSkeletonInstance();
	}

	TSharedSkeletalAnimationPtr Skeleton::CreateAnimation(const String& _animName, unsigned int _frameCount)
	{
		return m_Impl->CreateAnimation(_animName,_frameCount);
	}

	TSharedSkeletalAnimationPtr Skeleton::GetAnimation(const String& _name)
	{
		return m_Impl->GetAnimation(_name);
	}

	const String& Skeleton::GetName() const
	{
		return m_Impl->m_Name;
	}

	unsigned int Skeleton::GetBoneCount() const
	{
		return m_Impl->GetBoneCount();
	}
}