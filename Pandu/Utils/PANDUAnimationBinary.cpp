#include "PANDUAnimationBinary.h"

namespace Pandu
{
	bool AnimationBinary::LoadFromFile()
	{
		bool loaded = BinaryDataFile::LoadFromFile();

		if( loaded )
		{
			const BFileHeaderChunk &fileHeader = GetFileHeader();
			const BFileDataChunk &dataChunck = GetDataChunk();
			const BFileData &fileData = GetFileData();

			if( !dataChunck.HasSemantic(eABS_SKELETON_NAME) || !dataChunck.HasSemantic(eABS_ANIM_NAME) )
			{
				Pandu::gLogManager->WriteLog("Basic data required to build a mesh is not present in the file or file is corrupted...");
				return false;
			}

			unsigned int currIndex[eABS_Count];
			for( unsigned int i = 0 ; i < eABS_Count ; i++ )
			{
				currIndex[i] = 0;
			}

			BFileData::DataBlock* skelName = GetNextDataBlockBySemantic(fileData,eABS_SKELETON_NAME,currIndex[eABS_SKELETON_NAME]);

			if( skelName )
			{
				const char* skelNameChar = skelName->m_Data;
				if( skelNameChar )
				{
					TSharedSkeletonPtr skeleton = gSkeletonManager->GetSkeleton(skelNameChar);
					if( skeleton == NULL )
					{
						Pandu::gLogManager->WriteErrorLog("Please load the skeleton before loading animation");
						return false;
					}

					BFileData::DataBlock* animName = GetNextDataBlockBySemantic(fileData,eABS_ANIM_NAME,currIndex[eABS_ANIM_NAME]);

					if( animName )
					{
						const char* animNameChar = animName->m_Data;

						if( animNameChar )
						{
							BFileData::DataBlock* timeDataBlock = GetNextDataBlockBySemantic(fileData,eABS_ANIM_TIME_DATA,currIndex[eABS_ANIM_TIME_DATA]);
							float* timeData = (float*)timeDataBlock->m_Data;
							unsigned int frameCount = timeDataBlock->m_DataBlockHeader.m_DataCount;

							if( !timeData || frameCount == 0 )
							{
								Pandu::gLogManager->WriteErrorLog("Unable to create animation");
								return false;
							}

							TSharedSkeletalAnimationPtr animation = skeleton->CreateAnimation(animNameChar,frameCount);
							if( animation == NULL )
							{
								Pandu::gLogManager->WriteErrorLog("Unable to create animation");
								return false;
							}

							animation->SetAllTimesDataWithDataOwnership(timeData,frameCount);
							timeDataBlock->RemoveOwnershipFromData();

							LoadCreateAnimationDataToAnimation(animation,skeleton);
							return true;
						}
					}
				}
			}
		}
		return false;
	}

	void AnimationBinary::LoadCreateAnimationDataToAnimation(TSharedSkeletalAnimationPtr _animation, TSharedSkeletonPtr _skeleton)
	{
		BFileData &fileData = GetFileData();

		struct BoneAnimData
		{
			BoneAnimData()
				: transformDataBlock(NULL)
				, interpolateDataBlock(NULL)
			{

			}

			BFileData::DataBlock* transformDataBlock;
			BFileData::DataBlock* interpolateDataBlock;
		};

		const unsigned int boneCount = _skeleton->GetBoneCount();
		BoneAnimData* allBoneAnimData = new BoneAnimData[boneCount];

		for( unsigned int i = 0 ; i < fileData.DataBlocksCount() ; i++ )
		{
			BFileData::DataBlock* currDataBlock = fileData.GetDataBlock(i);
			if( currDataBlock->m_DataBlockHeader.m_DataBlockSemantic != eABS_SKELETON_NAME
				&& currDataBlock->m_DataBlockHeader.m_DataBlockSemantic != eABS_ANIM_NAME 
				&& currDataBlock->m_DataBlockHeader.m_DataBlockSemantic != eABS_ANIM_TIME_DATA)
			{
				if( currDataBlock->m_DataBlockHeader.m_DataBlockSemantic == eABS_BONE_ANIM_TRANSFORM_DATA
					|| currDataBlock->m_DataBlockHeader.m_DataBlockSemantic == eABS_BONE_ANIM_INTERPOLATE_DATA )
				{
					const unsigned int boneIndex = currDataBlock->GetUserData(0);//user data 0 is used as bone index for the animation file
					if( boneIndex >= boneCount )
					{
						PANDU_ERROR(false,"Bone index is higher than bone count");
						Pandu::gLogManager->WriteErrorLog("Bone index is greater than max bone in skeleton '" + _skeleton->GetName() + 
											"', in animation file '" + m_FileName + "'");
						continue;
					}

					switch(currDataBlock->m_DataBlockHeader.m_DataBlockSemantic)
					{
					case eABS_BONE_ANIM_TRANSFORM_DATA:
						{
							if(allBoneAnimData[boneIndex].transformDataBlock)
							{
								PANDU_ERROR(false,"Time data block already set");
								Pandu::gLogManager->WriteErrorLog(String("Transform data block is existed multiple times in bone '") + boneIndex 
									+ "' in skeleton '" + _skeleton->GetName() + "', in animation file '" + m_FileName + "'");
							}
							else
							{
								allBoneAnimData[boneIndex].transformDataBlock = currDataBlock;
							}
						}
						break;

					case eABS_BONE_ANIM_INTERPOLATE_DATA:
						{
							if(allBoneAnimData[boneIndex].interpolateDataBlock)
							{
								PANDU_ERROR(false,"Time data block already set");
								Pandu::gLogManager->WriteErrorLog(String("Interpolation data block is existed multiple times in bone '") + boneIndex 
									+ "' in skeleton '" + _skeleton->GetName() + "', in animation file '" + m_FileName + "'");
							}
							else
							{
								allBoneAnimData[boneIndex].interpolateDataBlock = currDataBlock;
							}
						}
						break;

					default:
						{

						}
						break;
					}
				}
				else
				{
					PANDU_ERROR(false,"Unknown semantic");
					Pandu::gLogManager->WriteErrorLog(String("Unknown data block in animation file '") + m_FileName + "', wasting file memory"
							+ ", please try to export the animation file clean next time");
				}
			}
		}

		for( unsigned int boneInd = 0 ; boneInd < boneCount ; boneInd++ )
		{
			if( allBoneAnimData[boneInd].interpolateDataBlock && allBoneAnimData[boneInd].transformDataBlock )
			{
				LoadAnimationFramesForBone(_animation,boneInd,*allBoneAnimData[boneInd].transformDataBlock,*allBoneAnimData[boneInd].interpolateDataBlock);
			}
			else
			{
				if( allBoneAnimData[boneInd].interpolateDataBlock  || allBoneAnimData[boneInd].transformDataBlock )
				{
					PANDU_ERROR(false,"Insufficient information");
					Pandu::gLogManager->WriteErrorLog(String("Insufficient animation information for bone '") + boneInd 
								+ "', in animation file '" + m_FileName + "'");
				}
			}
		}

		delete [] allBoneAnimData;
	}

	void AnimationBinary::LoadAnimationFramesForBone(TSharedSkeletalAnimationPtr _animation, unsigned int _boneIndex
						, const BFileData::DataBlock& _transformData, const BFileData::DataBlock& _interpolateData)
	{	
		const unsigned int transCount = _transformData.m_DataBlockHeader.m_DataCount/_transformData.m_DataBlockHeader.m_DataStride;
		const unsigned int intCount = _interpolateData.m_DataBlockHeader.m_DataCount/_interpolateData.m_DataBlockHeader.m_DataStride;
	
		if(  transCount != intCount )
		{
			PANDU_ERROR(false,"Transform and interpolate counts are not same");
			Pandu::gLogManager->WriteErrorLog(String("Transform and interpolate counts are not same for bone '") + _boneIndex 
				+ "', in animation file '" + m_FileName + "'");
			return;
		}

		const float* transData = (float*)_transformData.m_Data;
		const Interpolator::InterpolationMode* ipModeData = (Interpolator::InterpolationMode*)_interpolateData.m_Data;

		NodeAnimationFrame *allFrames = new NodeAnimationFrame[intCount];

		unsigned int t = 0;
		for( unsigned int i = 0 ; i < intCount ; i++ )
		{
			Matrix44 mat;
			for( unsigned int j = 0 ; j < _transformData.m_DataBlockHeader.m_DataStride ; j++ )
			{
				mat.arr[j] = transData[t++];
			}
			const Vector3 translate = mat.GetTranslate();
			//very unoptimized...
			const Matrix33 rotMat(mat.GetColumnVec3(0),mat.GetColumnVec3(1),mat.GetColumnVec3(2));
			const Quaternion rot(rotMat);

			allFrames[i].m_Position = translate;
			allFrames[i].m_Rotation = rot;
			allFrames[i].m_InterpolateMode = ipModeData[i];
		}

		_animation->SetAllFramesDataWithDataOwnership(_boneIndex,allFrames,intCount);
	}
}