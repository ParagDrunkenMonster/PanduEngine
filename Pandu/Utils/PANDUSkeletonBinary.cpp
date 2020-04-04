#include "PANDUSkeletonBinary.h"

namespace Pandu
{	
	bool SkeletonBinary::LoadFromFile()
	{
		bool loaded = BinaryDataFile::LoadFromFile();

		if( loaded )
		{
			const BFileHeaderChunk &fileHeader = GetFileHeader();
			const BFileDataChunk &dataChunck = GetDataChunk();
			const BFileData &fileData = GetFileData();
			
			if( !dataChunck.HasSemantic(eSBS_SKELETONNAME)  
				|| !dataChunck.HasSemantic(eSBS_BONENAME) || !dataChunck.HasSemantic(eSBS_TRANSFORM) 
				|| !dataChunck.HasSemantic(eSBS_CHILDCOUNT) || !dataChunck.HasSemantic(eSBS_CHILD) )
			{
				Pandu::gLogManager->WriteLog("Basic data required to build a mesh is not present in the file or file is corrupted...");
				return false;
			}

			return true;
		}
		return false;
	}

	TSharedSkeletonPtr SkeletonBinary::CreateSkeletonWithData()
	{
		if( !m_DataLoaded )
		{
			bool loaded = LoadFromFile();
			if( !loaded )
			{
				TSharedSkeletonPtr();
			}
		}

		const BFileHeaderChunk &fileHeader = GetFileHeader();
		const BFileDataChunk &dataChunk = GetDataChunk();
		const BFileData &fileData = GetFileData();

		unsigned int currIndex = 0;

		BFileData::DataBlock* meshNameDataBlock = GetNextDataBlockBySemantic(fileData,eSBS_SKELETONNAME,currIndex);

		TSharedSkeletonPtr newSkeleton = gSkeletonManager->CreateSkeleton(meshNameDataBlock->m_Data );

		SetSkeletonDataToSkeleton(newSkeleton);
	
		return newSkeleton;
	}

	void SkeletonBinary::SetSkeletonDataToSkeleton(TSharedSkeletonPtr _skeleton)
	{
		if( !m_DataLoaded )
		{
			Pandu::gLogManager->WriteLog("Error data not loaded...");
			return;
		}

		const BFileHeaderChunk &fileHeader = GetFileHeader();
		const BFileDataChunk &dataChunk = GetDataChunk();
		const BFileData &fileData = GetFileData();

		unsigned int currIndex[eSBS_Count];

		for( unsigned int i = 0 ; i < eSBS_Count ; i++ )
		{
			currIndex[i] = 0;
		}

		BFileData::DataBlock* boneNameDataBlock = GetNextDataBlockBySemantic(fileData,eSBS_BONENAME,currIndex[eSBS_BONENAME]);
		BFileData::DataBlock* boneTransformBlock = GetNextDataBlockBySemantic(fileData,eSBS_TRANSFORM,currIndex[eSBS_TRANSFORM]);


		if(  boneNameDataBlock )
		{
			String boneNameArray(boneNameDataBlock->m_Data);
			StringTokenizer tokenizer(boneNameArray,gNameSeperator);
			const float* transforms = (const float*)boneTransformBlock->m_Data;

			//Pandu::gLogManager->WriteErrorLog(boneNameArray);

			while( 1 )
			{
				String boneName = tokenizer.GetNextToken();
				Matrix44 currTransform;
				//memcpy(currTransform.arr,transforms,16);
				
				//Pandu::gLogManager->WriteErrorLog(boneName);
				for( int i = 0 ; i < 16 ; i++ )
				{
					currTransform.arr[i] = transforms[i];
				}

				Vector3 pos = currTransform.GetTranslate();

				_skeleton->CreateBoneInfo(boneName,currTransform);

				if( !tokenizer.HasNextToken() )
				{
					break;
				}

				transforms = transforms + 16;
			}
		}

		BFileData::DataBlock* childsCountDataBlock = GetNextDataBlockBySemantic(fileData,eSBS_CHILDCOUNT,currIndex[eSBS_CHILDCOUNT]);
		BFileData::DataBlock* childsDataBlock = GetNextDataBlockBySemantic(fileData,eSBS_CHILD,currIndex[eSBS_CHILD]);

		const unsigned int* childsCountData = (const unsigned int*)childsCountDataBlock->m_Data;
		const unsigned int* childsData = (const unsigned int*)childsDataBlock->m_Data;

		unsigned int childIndex = 0;
		for( unsigned int i = 0 ; i < childsCountDataBlock->m_DataBlockHeader.m_DataCount ; i++ )
		{
			const unsigned int currDhildCount = childsCountData[i];

			for( unsigned int j = 0 ; j < currDhildCount ; j++ )
			{
				const unsigned int child = childsData[childIndex++];
				_skeleton->AddChildToBone(i,child);
			}
		}

		Reset();
	}
}