/********************************************************************
	filename: 	PANDUColadaToPanduExporter
	author:		Parag Moni Boro
	
	purpose:	Game Engine created for learning
*********************************************************************/

#ifndef __PANDUColadaToPanduExporter_h__
#define __PANDUColadaToPanduExporter_h__

#include "PANDUCommandParameter.h"
#include <Utils/PANDUString.h>
#include <tinyxml.h>
#include <vector>
#include <Utils/PANDUGeometryBinary.h>
#include <Utils/PANDUSkeletonBinary.h>
#include <Utils/PANDUAnimationBinary.h>

#define MAX_FLOATS_PER_DATA_BLOCK MAX_NAME_SIZE/(sizeof(float))
#define MAX_INTS_PER_DATA_BLOCK MAX_NAME_SIZE/(sizeof(int))
#define MAX_UINTS_PER_DATA_BLOCK MAX_NAME_SIZE/(sizeof(unsigned int))

namespace Pandu
{
	class ColadaToPanduExporter
	{	
	public:

		enum ColadaVertexDataType
		{
			eCVDS_Position = 0,
			eCVDS_Normal,
			eCVDS_Tangent,
			eCVDS_Binormal,
			eCVDS_TexCoord0,
			eCVDS_SkeletonName,
			eCVDS_Joint,
			eCVDS_Weight,
			eCVDS_InvBindMatrix,
			eCVDS_BindShapeMatrix,

			eCVDS_Count,
		};

		enum AnimationTransformType
		{
			eATT_Transform = 0,

			eATT_Count,
		};

		struct ElementData
		{
			union
			{
				float floatValue[MAX_FLOATS_PER_DATA_BLOCK];
				int intValue[MAX_INTS_PER_DATA_BLOCK];
				unsigned int uintValue[MAX_UINTS_PER_DATA_BLOCK];
				char name[MAX_NAME_SIZE];
			};
		};

		typedef std::vector<ElementData> TVertexElementDataArray;

		struct ElementSourceData
		{
			ElementSourceData()
				: dataCount(0)
				, elementType(BinaryDataFile::eBDET_Count)
				, stride(0)
			{}

			TVertexElementDataArray elementDataArray;
			unsigned int dataCount;
			BinaryDataFile::BDataElementType elementType;
			unsigned short stride;
		};

		typedef std::vector<unsigned int> TUIntArray;

		struct ColladaSkeleton;

		struct ColladaGeometry
		{
			ColladaGeometry()
			{
				name[0] = '\0';
				id[0] = '\0';
				controllerName[0] = '\0';
				controllerId[0] = '\0';
				skeleton = NULL;
			}

			char name[MAX_NAME_SIZE];
			char id[MAX_NAME_SIZE];
			char controllerName[MAX_NAME_SIZE];
			char controllerId[MAX_NAME_SIZE];
			char skeletonName[MAX_NAME_SIZE];
			ElementSourceData elementSourceData[eCVDS_Count];
			TUIntArray vcountIndices[eCVDS_Count];
			TUIntArray vertexIndices[eCVDS_Count];
			const ColladaSkeleton* skeleton;
		};

		struct Bone
		{
			Bone()
			{
				name[0] = '\0';
				id[0] = '\0';
				sid[0] = '\0';
				index = 0;
				parent = NULL;
			}
			char name[MAX_NAME_SIZE];
			char id[MAX_NAME_SIZE];
			char sid[MAX_NAME_SIZE];
			unsigned int index;
			
			Bone* parent;
			std::vector<Bone*> childs;

			Matrix44 transform;
		};

		struct ColladaSkeleton
		{
			ColladaSkeleton()
			{
				name[0] = '\0';
				root = NULL;
			}
			~ColladaSkeleton()
			{
				unsigned int count = (unsigned int)allBones.size();
				for( unsigned int i = 0 ; i < count ; i++ )
				{
					delete allBones[i];
				}
				allBones.clear();
				root = NULL;
			}
			
			char name[MAX_NAME_SIZE];
			std::vector<Bone*> allBones;
			Bone* root;
		};

		struct ColladaSkeletonAnimationNode
		{
			const AnimationTransformType animationTransformType;
			const unsigned int boneIndex;

			ElementSourceData inputSourceData;
			ElementSourceData outputSourceData;
			ElementSourceData interpolationData;

			ColladaSkeletonAnimationNode(AnimationTransformType _animTransformType, unsigned int _boneInd)
				: animationTransformType(_animTransformType)
				, boneIndex(_boneInd)
			{

			}

			~ColladaSkeletonAnimationNode(){}
		};

		typedef std::map<const String,ColladaSkeletonAnimationNode*> TBoneNameAnimationNodeMap;

		struct ColladaSkeletonAnimationClip
		{
		public:

			ColladaSkeleton* skeleton;
			char name[MAX_NAME_SIZE];
			TBoneNameAnimationNodeMap animationNodes;

			ColladaSkeletonAnimationClip()
				: skeleton(NULL)
			{
				name[0] = '\0';
			}

			~ColladaSkeletonAnimationClip()
			{
				TBoneNameAnimationNodeMap::iterator itr = animationNodes.begin();

				for( ; itr != animationNodes.end() ; itr++ )
				{
					delete (*itr).second;
				}

				animationNodes.clear();
			}

			ColladaSkeletonAnimationNode* AddAnimationNode(const char* _boneId
												, AnimationTransformType _transformType
												, unsigned int _boneInd)
			{
				TBoneNameAnimationNodeMap::iterator itr = animationNodes.find(String(_boneId));
				if(itr == animationNodes.end())
				{
					std::pair<const String,ColladaSkeletonAnimationNode*> newAnimNode(_boneId,new ColladaSkeletonAnimationNode(_transformType,_boneInd) );
					animationNodes.insert(newAnimNode);
					return newAnimNode.second;
				}

				Pandu::gLogManager->WriteLog("Animation for this bone is already present in this skeleton");
				return NULL;
			}

			bool HasAnimationForBoneId(const char* _boneId) const
			{
				TBoneNameAnimationNodeMap::const_iterator itr = animationNodes.find(_boneId);
				if(itr != animationNodes.end())
				{
					return true;
				}
				return false;
			}
		};

		typedef std::vector<ColladaGeometry*> TColadaGeometryArray;
		typedef std::vector<GeometryBinary*> TGeometryBinaryArray;

		typedef std::vector<ColladaSkeleton*> TSkeletonArray;
		typedef std::vector<SkeletonBinary*> TSkeletonBinaryArray;

		typedef std::vector<ColladaSkeletonAnimationClip*> TSkeletonAnimationClipArray;
		typedef std::vector<AnimationBinary*> TAnimationBinaryArray;

	private:

		const String m_ExportPath;
		const String m_FileName;
		TColadaGeometryArray m_GeometryArray;
		TGeometryBinaryArray m_GeometryBinaryArray;
		TSkeletonArray m_Skeletons;
		TSkeletonBinaryArray m_SkeletonBinary;

		TSkeletonAnimationClipArray m_ColladaAnimClips;
		TAnimationBinaryArray m_AnimationBinary;
		float m_ScaleValue;
		Matrix44 m_UpConversionMatrix;

	public:

		ColadaToPanduExporter( const String& _exportPath,const String& _fileName );
		~ColadaToPanduExporter();

		void ExportToPandu(unsigned int _parameter );

	private:

		void LoadUpAxisConversion( const TiXmlNode* _coladaNode );

		// --------------------------------------------------------
		// Start export geometry functions
		// --------------------------------------------------------
		void LoadAllGeometries( const TiXmlNode* _coladaNode );
		bool LoadGeometry( const TiXmlNode* _geometryNode, ColladaGeometry& _oGeometry );
		bool LoadMesh( const TiXmlNode* _meshNode, ColladaGeometry& _oGeometry );
		bool LoadFaces( const TiXmlNode* _triangleNode, unsigned int highestOffset, int* _offsets, ColladaGeometry& _oGeometry );
		bool LoadVertices( const TiXmlNode* _meshNode, const TiXmlNode* _verticesNode
								, int _offsetAlreadyLoaded, int* _oOffsets, ColladaGeometry& _oGeometry);
		void LoadVertexElementData( const TiXmlNode* _meshNode, const char* const _sourceID, ElementSourceData& _oSourceData
									, bool _needConversion, bool _scaleNeeded);
		void LoadVertexSourceData( const TiXmlNode* _arrayNode, ElementSourceData& _oSourceData
									, bool _needConversion, bool _scaleNeeded ) const;
		void GetTechnique( const TiXmlNode* _sourceNode,String& _oSourceDataId, ElementSourceData& _oSourceData ) const;
		const TiXmlNode* GetAccessorById( const TiXmlNode* _techniqueNode ) const;
		const TiXmlNode* FindVerticesNodeById( const TiXmlNode* _meshNode, const char* const _id ) const;
		const TiXmlNode* FindSourceNodeById( const TiXmlNode* _parentNode, const char* const _id ) const;
		ColadaVertexDataType GetColadaVertexDataSemantic( const char* const _semanticName ) const;
		BinaryDataFile::BDataElementType GetDataElementType( const char* const _dataElementTypeName ) const;
		void AdjustAllGeometryBuffers();
		void AdjustGeometryBuffers(ColladaGeometry& _oGeometry);
		bool SkipVertexDataWhenAdjusting(ColadaVertexDataType _vertexDataType);
		void AlignBuffersWithBuffers(ElementSourceData& _sourceData, TUIntArray& _sourceIndsData
			, const ElementSourceData& _destData, const TUIntArray& _destIndsData);
		void AlignVerticesWithBuffers( ColladaGeometry& _oGeometry , const ElementSourceData& _destData, const TUIntArray& _destIndsData);
		void AllignArraysWithArray(float* _oNewSource, const float* _oldSource, unsigned int _oldSourceCount
			,const unsigned int* _oldSourceInds, unsigned int _oldSourceIndsCount , unsigned int _sourceStride
			,const unsigned int* _destArrayInds, unsigned int _destIndsCount);
		void AverageBuffers(ColladaGeometry& _oGeometry, ColadaVertexDataType _bufferType);

		void GetVCountAndStartIndOfVert(unsigned int _vertInd, const TUIntArray& _vCounts, unsigned int& _oVCount, unsigned int& _startIndIndex );
		// --------------------------------------------------------
		// End export geometry functions
		// --------------------------------------------------------

		// --------------------------------------------------------
		// Start export controllers functions
		// --------------------------------------------------------
		void LoadAllControllers( const TiXmlNode* _coladaNode );
		void LoadAllSkin( const TiXmlNode* _controllerNode );
		const ColladaSkeleton* GetSkeletonHavingAllBoneSIDS(const TVertexElementDataArray& _dataArray, unsigned int _dataCount) const;
		bool DoesSkeletonHaveAllBoneSIDS(const ColladaSkeleton& _skeleton, const TVertexElementDataArray& _dataArray, unsigned int _dataCount) const;
		bool DoesSkeletonContainBoneSID(const ColladaSkeleton& _skeleton, const char* _boneSID ) const;
		int GetBoneIndexOfSkeletonFromBoneSID(const ColladaSkeleton& _skeleton, const char* _boneSID ) const;
		bool LoadVertexWeightsSources( const TiXmlNode* _skinNode, const TiXmlNode* _vertWeightNode,int* _oOffsets, int& _oMaxOffset, ColladaGeometry& _oGeometry ) const;
		bool LoadSource( const TiXmlNode* _parentNode,const char* _sourceId, ElementSourceData& _oSourceData
										, bool _conversionNeeded , bool _scaleNeeded ) const;
		ColladaGeometry* GetColadaGeometryBasedOnId( const char* _id );
		// --------------------------------------------------------
		// End export controllers functions
		// --------------------------------------------------------

		bool LoadIndices( const TiXmlNode* _parentNode, const int _indTypeOffset[eCVDS_Count],int _maxOffset, ColladaGeometry& _oGeometry ) const;
		const TiXmlNode* GetChildNodeWithID( const TiXmlNode* _parentNode, const char* const _id, String& _oNodeName) const;

		// --------------------------------------------------------
		// Start export skeleton functions
		// --------------------------------------------------------
		void LoadAllSkeleton( const TiXmlNode* _coladaNode );
		bool LoadSkeleton( const TiXmlNode* _visualScene , ColladaSkeleton& _oSkeleton) const;
		void LoadAllChilds( const TiXmlNode* _parent, Bone* _parentBone, ColladaSkeleton& _oSkeleton ) const;
		bool LoadTransformForNode( const TiXmlNode* _node, Matrix44& _transform ) const;
		Bone* FindBoneWithId(const ColladaSkeleton& _oSkeleton, const char* const _id ) const;
		const TiXmlNode* GetNextNodeOfType( const TiXmlNode* _parent, const TiXmlNode* _currNode, const char* const _type ) const;
		// --------------------------------------------------------
		// End export skeleton functions
		// --------------------------------------------------------

		// --------------------------------------------------------
		// Start Geometry Conversion functions
		// --------------------------------------------------------
		void ConverAllGeometriesToPANDUGeometryBinary();
		bool ConvertGeometryToPANDUGeometryBinary(const ColladaGeometry& _coladaGeometry, GeometryBinary& _oGeometryBinary);
		void ConverElementSourceToPANDUGeometryBinary(const ColladaGeometry& _colladaGeometry, const ElementSourceData& _elementSourceData, ColadaVertexDataType _vDataType, GeometryBinary& _oGeometryBinary,bool& _oPositionSemanticFound);
		void DumpAllGeometryBinaryToFile() const;

		void ConvertAllSkeletonsToPANDUSkeletonBinary();
		bool ConvertSkeletonToPANDUSkeletonBinary(const ColladaSkeleton& _colladaSkeleton, SkeletonBinary& _oSkeletonBinary);
		bool ConvertChildsToPANDUSkeletonBinary( const ColladaSkeleton& _colladaSkeleton, SkeletonBinary& _oSkeletonBinary, unsigned int _totalChildCount);
		bool ConverChildCountToPANDUSkeletonBinary( const ColladaSkeleton& _colladaSkeleton, SkeletonBinary& _oSkeletonBinary, unsigned int& _oTotalChildCount);
		bool ConverTransformsToPANDUSkeletonBinary( const ColladaSkeleton& _colladaSkeleton, SkeletonBinary& _oSkeletonBinary);
		bool ConverBonesNameIdToPANDUSkeletonBinary( const ColladaSkeleton& _colladaSkeleton, bool boneID, SkeletonBinary& _oSkeletonBinary);
		void DumpAllSkeletonsBinaryToFile() const;
		// --------------------------------------------------------
		// End Geometry Conversion functions
		// --------------------------------------------------------

		// --------------------------------------------------------
		// Start export animation
		// --------------------------------------------------------
		void LoadAllAnimations( const TiXmlNode* _coladaNode );
		void LoadAnimation( const TiXmlNode* _animation);
		bool LoadSourcesFromSampler(const char* boneId, AnimationTransformType _tt,const TiXmlNode* _animNode, const TiXmlNode* _samplerNode, ColladaSkeletonAnimationClip& _oSklAnimClip) const;
		AnimationTransformType GetAnimationTransformType(const char* _charTransformType) const;
		ColladaSkeletonAnimationClip* FindSkeletonAnimClipForBone(const char* _boneId);
		ColladaSkeletonAnimationClip* FindAnimationClipForSkeleton(const char* _sklName, const char* _clipName) const;
		const TiXmlNode* FindSamplerSourceById(const TiXmlNode* _animNode, const char* _samplerSourceId) const;

		void ConvertAllAnimationToPANDUAnimationBinary();
		bool ConvertAnimationToPANDUAnimationBinary(const ColladaSkeletonAnimationClip& _colladaAnim, AnimationBinary& _oAnimBinary);
		bool ConvertAnimationNodeToPanduAnimationBinary(const String& _boneName, const ColladaSkeletonAnimationNode& _coladaAnimNode, AnimationBinary& _oAnimBinary);
	
		void DumpAllAnimationBinaryToFile() const;

		void ConvertVector3(float* _data, bool _convert, bool _scale) const;
		void ConvertMatrix44(float* _data, bool _convert, bool _scale) const;
	};
}

#endif