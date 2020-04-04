#include "PANDUColadaToPanduExporter.h"
#include <sstream>
#include <Utils/PANDUStringTokenizer.h>
#include <Math/PANDUQuaternion.h>
#include <Math/PANDUMathUtils.h>
#include <Graphics/PANDUInterpolator.h>

namespace
{
	using namespace Pandu;

	//Local constants for geometry binary
	const GeometryBinary::GBSemantic kGBSemanticBasedOnColadaVertexDataType[ColadaToPanduExporter::eCVDS_Count] =	
													{
														GeometryBinary::eGBS_POSITION,			//eCVDS_Position,
														GeometryBinary::eGBS_NORMAL,			//eCVDS_Normal,
														GeometryBinary::eGBS_SKELETONNAME,		//eCVDS_SkeletonName
														GeometryBinary::eGBS_BONES,				//eCVDS_Joint,
														GeometryBinary::eGBS_BONEWEIGHTS,		//eCVDS_Weight,
														GeometryBinary::eGBS_INVBINDMATRIX,		//eCVDS_InvBindMatrix,
														GeometryBinary::eGBS_BINDSHAPEMATRIX,	//eCVDS_BindShapeMatrix,
													};


	const BinaryDataFile::BDataElementType kGBDataElementTypeBasedOnDataElementType[ColadaToPanduExporter::eDET_Count] = 
													{
														BinaryDataFile::eBDET_Float,		//eDET_Float,
														BinaryDataFile::eBDET_Int,			//eDET_Int,
														BinaryDataFile::eBDET_UnsignedInt,	//eDET_UnsignedInt,
														BinaryDataFile::eBDET_Float,		//eDET_Float4x4,
														BinaryDataFile::eBDET_Count,		//eDET_CStr,
													};

	//Geometry binary data element sizes
	const unsigned int kGBDataElementTypeSizes[BinaryDataFile::eBDET_Count] = 
													{
														sizeof(float),				//BinaryDataFile::eBDET_Float,
														sizeof(int),				//BinaryDataFile::eBDET_Int,
														sizeof(unsigned int),		//BinaryDataFile::eBDET_UnsignedInt,
														sizeof(char),				//BinaryDataFile::eBDET_Char,
													};

	//-----------------------------------------------------------------------------------
	//-----------------------------------------------------------------------------------
	//Local constants for skeleton binary

	const SkeletonBinary::SBSemantic kSBSemanticBasedOnColadaVertexDataType[ColadaToPanduExporter::eCVDS_Count] =	
													{
														SkeletonBinary::eSBS_None,			//eCVDS_Position,
														SkeletonBinary::eSBS_None,			//eCVDS_Normal,
														SkeletonBinary::eSBS_None,			//eCVDS_SkeletonName,
														SkeletonBinary::eSBS_None,			//eCVDS_Joint,
														SkeletonBinary::eSBS_None,			//eCVDS_Weight,
														SkeletonBinary::eSBS_None,			//eCVDS_InvBindMatrix,
														SkeletonBinary::eSBS_None,			//eCVDS_BindShapeMatrix,
													};


	const BinaryDataFile::BDataElementType kSBDataElementTypeBasedOnDataElementType[ColadaToPanduExporter::eDET_Count] = 
													{
														BinaryDataFile::eBDET_Float,		//eDET_Float,
														BinaryDataFile::eBDET_Int,			//eDET_Int,
														BinaryDataFile::eBDET_UnsignedInt,	//eDET_UnsignedInt,
														BinaryDataFile::eBDET_Count,		//eDET_Float4x4,
														BinaryDataFile::eBDET_Count,		//eDET_CStr,
													};
	//Skeleton binary data element sizes
	const unsigned int kSBDataElementTypeSizes[BinaryDataFile::eBDET_Count] = 
													{
														sizeof(float),				//BinaryDataFile::eBDET_Float,
														sizeof(int),				//BinaryDataFile::eBDET_Int,
														sizeof(unsigned int),		//BinaryDataFile::eBDET_UnsignedInt,
														sizeof(char),				//BinaryDataFile::eBDET_Char,
														sizeof(char),				//BinaryDataFile::eBDET_Name,
														sizeof(float),				//BinaryDataFile::eBDET_Float4x4
													};

	//-----------------------------------------------------------------------------------
	//-----------------------------------------------------------------------------------
	//Local constants for animation binary

	const char* const kAnimationTransformType[ColadaToPanduExporter::eATT_Count] = 
													{
														"transform",				//eATT_Transform
													};
}

namespace Pandu
{
	const char* const ColadaToPanduExporter::kColadaVertexDataTypeNames[eCVDS_Count] =	
													{
														"POSITION",						//eCVDS_Position
														"NORMAL",						//eCVDS_Normal
														"SKELETONNAME",					//eCVDS_SkeletonName
														"JOINT",						//eCVDS_Joint
														"WEIGHT",						//eCVDS_Weight
														"INV_BIND_MATRIX",				//eCVDS_InvBindMatrix
														"BIND_SHAPE_MATRIX",			//eCVDS_BindShapeMatrix
													};

	const char* const ColadaToPanduExporter::kDataElementTypeNames[eDET_Count] =	
													{
														"float",			//eDET_Float
														"int",				//eDET_Int
														"unsigned int",		//eDET_UnsignedInt
														"float4x4",			//eDET_Float4x4
														"name",				//eDET_CStr
													};

	const unsigned int ColadaToPanduExporter::kDataElementTypeCountPerElementData[eDET_Count] =	
													{
														MAX_FLOATS_PER_DATA_BLOCK,	//eDET_Float
														MAX_INTS_PER_DATA_BLOCK,	//eDET_Int
														MAX_UINTS_PER_DATA_BLOCK,	//eDET_UnsignedInt
														MAX_FLOATS_PER_DATA_BLOCK,	//eDET_Float4x4
														1,							//eDET_CStr
													};

	

	ColadaToPanduExporter::ColadaToPanduExporter( const String& _exportPath,const String& _fileName )
		: m_ExportPath(_exportPath)
		, m_FileName(_fileName)
	{

	}

	ColadaToPanduExporter::~ColadaToPanduExporter()
	{
		const unsigned int coladaGeometryCount = (unsigned int)m_GeometryArray.size();

		for( unsigned int i = 0 ; i < coladaGeometryCount ; i++ )
		{
			delete m_GeometryArray[i];
		}

		m_GeometryArray.clear();

		const unsigned int geometryBinaryCount = (unsigned int)m_GeometryBinaryArray.size();

		for( unsigned int i = 0 ; i < geometryBinaryCount ; i++ )
		{
			delete m_GeometryBinaryArray[i];
		}

		m_GeometryBinaryArray.clear();


		const unsigned int skeletonCount = (unsigned int)m_Skeletons.size();

		for( unsigned int i = 0 ; i < skeletonCount ; i++ )
		{
			delete m_Skeletons[i];
		}
		m_Skeletons.clear();

		const unsigned int skBinaryCount = (unsigned int)m_SkeletonBinary.size();

		for( unsigned int i = 0 ; i < skBinaryCount ; i++ )
		{
			delete m_SkeletonBinary[i];
		}

		m_SkeletonBinary.clear();

		const unsigned int animClipCount = (unsigned int)m_ColladaAnimClips.size();

		for( unsigned int i = 0 ; i < animClipCount ; i++ )
		{
			delete m_ColladaAnimClips[i];
		}

		m_ColladaAnimClips.clear();

		const unsigned int animBinaryCount = (unsigned int)m_AnimationBinary.size();

		for( unsigned int i = 0 ; i < animBinaryCount ; i++ )
		{
			delete m_AnimationBinary[i];
		}

		m_AnimationBinary.clear();
	}

	void ColadaToPanduExporter::ExportToPandu( unsigned int _parameter )
	{
		TiXmlDocument doc( m_FileName.CString() );
		bool loadOkay = doc.LoadFile();

		if ( !loadOkay )
		{
			Pandu::gLogManager->WriteLog("Could not load file '" + m_FileName + "'. Error='" 
				+ doc.ErrorDesc() + "'. Exiting." );
			return;
		}

		Pandu::gLogManager->WriteLog("Loading file '" + m_FileName + "'..." );
		
		TiXmlNode* coladaNode = doc.FirstChild( "COLLADA" );
		
		if( coladaNode )
		{
			Pandu::gLogManager->WriteLog("COLLADA Node Found...");
			LoadAllGeometries(coladaNode);
			LoadAllSkeleton(coladaNode);
			LoadAllControllers(coladaNode);
			LoadAllAnimations(coladaNode);

			ConverAllGeometriesToPANDUGeometryBinary();
			DumpAllGeometryBinaryToFile();

			ConvertAllSkeletonsToPANDUSkeletonBinary();
			DumpAllSkeletonsBinaryToFile();

			ConvertAllAnimationToPANDUAnimationBinary();
			DumpAllAnimationBinaryToFile();
		}
		else
		{
			Pandu::gLogManager->WriteLog("Not valid COLLADA document...");
			return;
		}
	}

	// --------------------------------------------------------
	// Start export geometry functions
	// --------------------------------------------------------
	void ColadaToPanduExporter::LoadAllGeometries( const TiXmlNode* _coladaNode )
	{
		const TiXmlNode* libraryGeometryNode = _coladaNode->FirstChild("library_geometries");
		
		if( libraryGeometryNode )
		{
			Pandu::gLogManager->WriteLog("Library geometry found...");
			
			const TiXmlNode* firstGeometry = libraryGeometryNode->FirstChild("geometry");

			if( firstGeometry )
			{
				ColladaGeometry* geometry = new ColladaGeometry;
				bool geometryExported = LoadGeometry(firstGeometry,*geometry);
				if( geometryExported )
				{
					const TiXmlElement* geometryelement = firstGeometry->ToElement();
					const char* geometryID = geometryelement->Attribute("id");

					if( strlen(geometryID) >= MAX_NAME_SIZE )
					{
						assert(false);
						Pandu::gLogManager->WriteLog(String("ERROR - ID has more than ") + (MAX_NAME_SIZE-1) + " characters");
					}

					strcpy_s(geometry->id,MAX_NAME_SIZE,geometryID);

					m_GeometryArray.push_back(geometry);
				}
				else
				{
					delete geometry;
				}
			}
			else
			{
				Pandu::gLogManager->WriteLog("No geometry in library geometry...");
			}
		}
		else
		{
			Pandu::gLogManager->WriteLog("No library geometry exists...");
		}
	}

	bool ColadaToPanduExporter::LoadGeometry( const TiXmlNode* geometryNode, ColladaGeometry& _oGeometry )
	{
		const TiXmlNode* meshNode = geometryNode->FirstChild("mesh");
		const TiXmlElement* geometryNodeElement = geometryNode->ToElement();

		if( geometryNodeElement )
		{
			const char* geometryName = geometryNodeElement->Attribute("name");
			if( geometryName )
			{
				Pandu::gLogManager->WriteLog(String("Geometry name '") + geometryName + "'...");
			}
			else
			{
				const char* geometryID = geometryNodeElement->Attribute("id");
				if( geometryID )
				{
					Pandu::gLogManager->WriteLog(String("Geometry name '") + geometryID + "'...");
				}
				else
				{
					return false;
				}
			}
		}
		else
		{
			Pandu::gLogManager->WriteLog("Geometry node element not found...");
			return false;
		}

		if( meshNode )
		{
			Pandu::gLogManager->WriteLog("Mesh found in geometry...");
			if( LoadMesh( meshNode, _oGeometry) )
			{
				const char* geometryName = geometryNodeElement->Attribute("name");

				if( !geometryName )
				{
					geometryName = geometryNodeElement->Attribute("id");
				}
				
				if( strlen(geometryName) >= MAX_NAME_SIZE )
				{
					assert(false);
					Pandu::gLogManager->WriteLog(String("ERROR - Name has more than ") + (MAX_NAME_SIZE-1) + " characters");
				}

				strcpy_s(_oGeometry.name,MAX_NAME_SIZE,geometryName);

				return true;
			}
		}
		else
		{
			Pandu::gLogManager->WriteLog("No mesh in geometry node...");
			
		}

		return false;
	}

	bool ColadaToPanduExporter::LoadMesh( const TiXmlNode* _meshNode, ColladaGeometry& _oGeometry )
	{
		//not done properly ... currently only VERTEX semantic is used....
		const TiXmlNode* triangles = _meshNode->FirstChild("triangles");//polylist

		if( !triangles )
		{
			triangles = _meshNode->FirstChild("polygons");//
		}
		
		if( triangles )
		{
			Pandu::gLogManager->WriteLog("Triangles found...");

			const TiXmlNode* inputNode = triangles->FirstChild("input");
			const TiXmlNode* verticesIpNode = inputNode;

			if( inputNode )
			{
				bool verticesFound = false;
				int highestOffset = 0;
				int currOffset = 0;

				while( inputNode )
				{
					const TiXmlElement* inputElement = inputNode->ToElement();
					const char* sementic = inputElement->Attribute("semantic");
					const char* offsetstr = inputElement->Attribute("offset",&currOffset);

					if( currOffset > highestOffset )
					{
						highestOffset = currOffset;
					}

					if( !verticesFound && strcmp(sementic,"VERTEX") == 0 )
					{
						verticesFound = true;
						verticesIpNode = inputNode;
					}

					inputNode = inputNode->NextSibling("input");
				}

				if(verticesFound)
				{
					const TiXmlElement* inputElement = verticesIpNode->ToElement();
					const char* source = inputElement->Attribute("source");
					const char* verticesSourceID = ++source;

					Pandu::gLogManager->WriteLog(String("Vertex source name '") + verticesSourceID + "'...");

					const TiXmlNode* verticesNode = FindVerticesNodeById(_meshNode,verticesSourceID);

					if( verticesNode )
					{
						if( LoadVertices(_meshNode,verticesNode,_oGeometry) )
						{
							return LoadFaces(triangles,highestOffset,_oGeometry);
						}
					}
					else
					{
						Pandu::gLogManager->WriteLog("\nNo vertices for this mesh...");
						return false;
					}
				}
				else
				{
					Pandu::gLogManager->WriteLog("No VERTEX NODE found for this mesh...");
					return false;
				}

				return false;
			}
			else
			{
				Pandu::gLogManager->WriteLog("No input for this mesh...");
			}
		}
		else
		{
			Pandu::gLogManager->WriteLog("Only triangles supported, please convert mesh into trialgles...");
		}

		return false;
	}

	bool ColadaToPanduExporter::LoadFaces( const TiXmlNode* _triangleNode, unsigned int highestOffset, ColladaGeometry& _oGeometry )
	{
		const TiXmlElement* triangleElement = _triangleNode->ToElement();
		std::stringstream triangleCountStrm(triangleElement->Attribute("count"));
		unsigned int triangleCount;
		triangleCountStrm >> triangleCount;
		unsigned int currTriCount = 0;

		const TiXmlNode* pNode = _triangleNode->FirstChild("p");
		
		//not required the while loop in open collada, its required only in 3ds max collada file
		while( pNode && currTriCount < triangleCount)
		{
			const TiXmlElement* pNodeElement = pNode->ToElement();
			const char* text = pNodeElement->GetText();
			//const char* countText = pNodeElement->

			std::stringstream stringStream(text);
			
			unsigned int count = 0;
			const unsigned int totalOffset = highestOffset + 1;
		
			while( !stringStream.eof() )
			{
				int data = 0;
				stringStream >> data;
				
				if( (count % totalOffset) == 0 )
				{
					_oGeometry.vertexIndices[eCVDS_Position].push_back(data);
				}
				count++;

				if( (count % 3) == 0 )
				{
					currTriCount++;
				}
			}

			pNode = pNode->NextSibling("p");
		}


		if( currTriCount > 0 )
		{
			return true;
		}

		return false;
	}

	bool ColadaToPanduExporter::LoadVertices( const TiXmlNode* _meshNode, const TiXmlNode* _verticesNode, ColladaGeometry& _oGeometry )
	{
		ElementSourceData* allData = _oGeometry.elementSourceData;

		const TiXmlNode* inputNode = _verticesNode->FirstChild("input");

		while( inputNode )
		{
			const TiXmlElement* inputElement = inputNode->ToElement();
			const char* semanticStr = inputElement->Attribute("semantic");

			ColadaVertexDataType semantic = GetColadaVertexDataSemantic(semanticStr);

			if( semantic >= 0 && semantic < eCVDS_Count )
			{
				const char* source = inputElement->Attribute("source");
				const char* sourceId = ++source;
				LoadVertexElementData(_meshNode, sourceId, allData[semantic]);
			}

			inputNode = inputNode->NextSibling("input");
		}

		Pandu::gLogManager->WriteLog(String("Vertices Loaded count = ") + (int)allData[eCVDS_Position].elementDataArray.size() + " ..." );
		Pandu::gLogManager->WriteLog(String("Normals Loaded count = ") + (int)allData[eCVDS_Normal].elementDataArray.size() + " ...");

		if( (int)allData[eCVDS_Position].elementDataArray.size() > 0 )
		{
			return true;
		}

		return false;
	}

	void ColadaToPanduExporter::LoadVertexElementData( const TiXmlNode* _meshNode, const char* const _sourceID, ElementSourceData& _oSourceData )
	{
		const TiXmlNode* sourceNode = FindSourceNodeById(_meshNode,_sourceID);

		if( sourceNode )
		{
			const TiXmlElement* sourceElement = sourceNode->ToElement();
			const char* sourceId = sourceElement->Attribute("id");
			const char* name = sourceElement->Attribute("name");

			if( name )
			{
				Pandu::gLogManager->WriteLog(String("Source node id '") + sourceId + "' and named '" + name + "' found...");
			}
			else
			{
				Pandu::gLogManager->WriteLog(String("Source node id '") + sourceId + "' and named '' found...");
			}
			

			String sourceDataId;
			GetTechnique(sourceNode,sourceDataId,_oSourceData);

			if( _oSourceData.stride > 0 && _oSourceData.elementType >= 0 && _oSourceData.elementType < eDET_Count )
			{
				const TiXmlNode* arrayNode = sourceNode->FirstChild("float_array");

				if( arrayNode )
				{
					LoadVertexSourceData(arrayNode,_oSourceData);
				}
			}
		}
	}

	void ColadaToPanduExporter::LoadVertexSourceData( const TiXmlNode* _arrayNode , ElementSourceData& _oSourceData ) const
	{
		const TiXmlElement* arrayNodeElement = _arrayNode->ToElement();
		int dataCount = 0;
		const char* countText = arrayNodeElement->Attribute("count",&dataCount);
		_oSourceData.dataCount = (unsigned int)dataCount;

		const char* text = arrayNodeElement->GetText();
		const DataElementType type = _oSourceData.elementType;

		if( type != eDET_CStr )
		{
			std::stringstream stringStream(text);
			ElementData elementData;

			while( !stringStream.eof() )
			{
				for( unsigned int i = 0 ; i < kDataElementTypeCountPerElementData[type] ; i++ )
				{
					switch(type)
					{
					case eDET_Float:
					case eDET_Float4x4:
						{
							stringStream >> elementData.floatValue[i];
						}
						break;

					case eDET_Int:
						{
							stringStream >> elementData.intValue[i];
						}
						break;

					case eDET_UnsignedInt:
						{
							stringStream >> elementData.uintValue[i];
						}
						break;

					default:
						{
						}
						break;
					}

					if( stringStream.eof() )
					{
						break;
					}
				}
				_oSourceData.elementDataArray.push_back(elementData);
			}
		}
		else
		{
			StringTokenizer tokenizer(text,' ');

			ElementData elementData;

			while( tokenizer.HasNextToken() )
			{
				const String token = tokenizer.GetNextToken();

				if( strlen(token.CString()) >= MAX_NAME_SIZE )
				{
					assert(false);
					Pandu::gLogManager->WriteLog(String("ERROR - Name has more than ") + (MAX_NAME_SIZE-1) + " characters");
				}
				strcpy_s(elementData.name,MAX_NAME_SIZE,token.CString());

				_oSourceData.elementDataArray.push_back(elementData);
			}
		}
	}

	void ColadaToPanduExporter::GetTechnique( const TiXmlNode* _sourceNode, String& _oSourceDataId, ElementSourceData& _oSourceData ) const
	{
		const TiXmlNode* techniqueNode = _sourceNode->FirstChild("technique_common");

		if( techniqueNode )
		{
			//array id need to be passed in GetAccessorById function
			//since for time only one array is allowed per source
			//_id is ignored..
			const TiXmlNode* accessorNode = GetAccessorById(techniqueNode);

			if( accessorNode )
			{
				const TiXmlElement* accessorElement = accessorNode->ToElement();
				const char* stride = accessorElement->Attribute("stride");
				if( stride )
				{
					sscanf_s(stride,"%d",&_oSourceData.stride);
				}
				else
				{
					_oSourceData.stride = 1;
				}
				const char* sourceId = accessorElement->Attribute("source");
				_oSourceDataId = ++sourceId;

				const TiXmlNode* paramNode = accessorNode->FirstChild("param");

				if( paramNode )
				{
					const TiXmlElement* paramElement = paramNode->ToElement();
					const char* paramTypeStr = paramElement->Attribute("type");
					_oSourceData.elementType = GetDataElementType(paramTypeStr);
				}
				else
				{
					_oSourceData.elementType = eDET_Count;
					_oSourceData.stride = 0;
				}
			}	
			else
			{
				_oSourceData.elementType = eDET_Count;
				_oSourceData.stride = 0;
			}
		}
		else
		{
			_oSourceData.elementType = eDET_Count;
			_oSourceData.stride = 0;
		}
	}

	const TiXmlNode* ColadaToPanduExporter::GetAccessorById( const TiXmlNode* _techniqueNode ) const
	{
		const TiXmlNode* accessorNode = _techniqueNode->FirstChild("accessor");
		return accessorNode;
	}

	const TiXmlNode* ColadaToPanduExporter::FindVerticesNodeById( const TiXmlNode* _meshNode, const char* const _id ) const
	{
		const TiXmlNode* verticesNode = _meshNode->FirstChild("vertices");

		while( verticesNode )
		{
			const TiXmlElement* inputElement = verticesNode->ToElement();
			const char* attributeID = inputElement->Attribute("id");

			if( strcmp(attributeID,_id) == 0 )
			{
				return verticesNode;
			}

			verticesNode = verticesNode->NextSibling("vertices");
		}

		return NULL;
	}

	const TiXmlNode* ColadaToPanduExporter::FindSourceNodeById( const TiXmlNode* _parentNode, const char* const _id ) const
	{
		const TiXmlNode* sourceNode = _parentNode->FirstChild("source");

		while( sourceNode )
		{
			const TiXmlElement* sourceElement = sourceNode->ToElement();
			const char* attributeID = sourceElement->Attribute("id");

			if( strcmp(attributeID,_id) == 0 )
			{
				return sourceNode;
			}

			sourceNode = sourceNode->NextSibling("source");
		}

		return NULL;
	}

	ColadaToPanduExporter::ColadaVertexDataType ColadaToPanduExporter::GetColadaVertexDataSemantic( const char* const _semanticName ) const
	{
		for( int i = 0 ; i < eCVDS_Count ; i++ )
		{
			if( strcmp(_semanticName,kColadaVertexDataTypeNames[i]) == 0 )
			{
				return (ColadaVertexDataType)i;
			}
		}

		return eCVDS_Count;
	}

	ColadaToPanduExporter::DataElementType ColadaToPanduExporter::GetDataElementType( const char* const _dataElementTypeName ) const
	{
		for( int i = 0 ; i < eDET_Count ; i++ )
		{
			if( strcmp(_dataElementTypeName,kDataElementTypeNames[i]) == 0 )
			{
				return (DataElementType)i;
			}
		}

		return eDET_Count;
	}
	// --------------------------------------------------------
	// End export geometry functions
	// --------------------------------------------------------


	// --------------------------------------------------------
	// Start export controllers functions
	// --------------------------------------------------------

	void ColadaToPanduExporter::LoadAllControllers( const TiXmlNode* _coladaNode )
	{
		const TiXmlNode* libraryControllers = _coladaNode->FirstChild("library_controllers");

		if( libraryControllers )
		{
			const TiXmlNode* firstController = libraryControllers->FirstChild("controller");

			if( firstController )
			{
				//while( firstController )
				{
					LoadAllSkin( firstController );
					//firstController = firstController->NextSibling("controller");
				}
			}
		}
	}

	void ColadaToPanduExporter::LoadAllSkin( const TiXmlNode* _controllerNode )
	{
		const TiXmlNode* firstSkin = _controllerNode->FirstChild("skin");
		const TiXmlElement* coltrollerElement = _controllerNode->ToElement();
		const char* controllerName = coltrollerElement->Attribute("name");
		const char* controllerId = coltrollerElement->Attribute("id");

		int indTypeOffset[eCVDS_Count];
		for( int i = 0 ; i < eCVDS_Count ; i++ )indTypeOffset[i] = -1;
		int maxOffset = 0;

	//	while( firstSkin )
		{
			const TiXmlElement* skinElement = firstSkin->ToElement();
			const char* source = skinElement->Attribute("source");
			const char* geometryID = ++source;

			ColladaGeometry* geometry = GetColadaGeometryBasedOnId(geometryID);

			if( geometry )
			{
				const TiXmlNode* bindShapeMatNode = firstSkin->FirstChild("bind_shape_matrix");

				if( bindShapeMatNode )
				{
					//const TiXmlElement* bindShapeMatElement = bindShapeMatNode->ToElement();
					//const char* data = bindShapeMatElement->GetText();
					
					geometry->elementSourceData[eCVDS_BindShapeMatrix].elementType = eDET_Float4x4;
					geometry->elementSourceData[eCVDS_BindShapeMatrix].stride = 16;
					LoadVertexSourceData(bindShapeMatNode,geometry->elementSourceData[eCVDS_BindShapeMatrix]);
					geometry->elementSourceData[eCVDS_BindShapeMatrix].dataCount = 16;
				}

				const TiXmlNode* vertWeightsNode = firstSkin->FirstChild("vertex_weights");

				if( vertWeightsNode && LoadVertexWeightsSources(firstSkin,vertWeightsNode,(&indTypeOffset[0]),maxOffset,*geometry) )
				{
					if( controllerName )
					{
						strcpy_s(geometry->controllerName,MAX_NAME_SIZE,controllerName);
					}
					else
					{
						strcpy_s(geometry->controllerName,MAX_NAME_SIZE,controllerId);
					}
					
					strcpy_s(geometry->controllerId,MAX_NAME_SIZE,controllerId);

					const TiXmlNode* jointsNodes = firstSkin->FirstChild("joints");

					int tempIndTypeOffset[eCVDS_Count];
					for( int i = 0 ; i < eCVDS_Count ; i++ )tempIndTypeOffset[i] = -1;
					int tempMaxOffset = 0;
					if( !( jointsNodes && LoadVertexWeightsSources(firstSkin,jointsNodes,&tempIndTypeOffset[0],tempMaxOffset,*geometry)) )
					{
						Pandu::gLogManager->WriteLog("Unable to load vertex weights");
					}

					if( LoadIndices(vertWeightsNode,indTypeOffset,maxOffset,*geometry) )
					{
						const ElementSourceData& jntsElmntData = geometry->elementSourceData[eCVDS_Joint];

						const ColladaSkeleton* skl = GetSkeletonHavingAllBoneSIDS(jntsElmntData.elementDataArray,jntsElmntData.dataCount);
						
						ElementSourceData& sklNameElmntData = geometry->elementSourceData[eCVDS_SkeletonName];

						sklNameElmntData.dataCount = 1;
						sklNameElmntData.elementType = eDET_CStr;
						sklNameElmntData.stride = 1;

						ElementData elementData;
						strcpy_s(elementData.name,MAX_NAME_SIZE,skl->name);
						
						sklNameElmntData.elementDataArray.push_back(elementData);

						geometry->skeleton = skl;
					}
				}
				else
				{
					Pandu::gLogManager->WriteLog("Unable to load vertex weights");
				}
			}
			else
			{
				Pandu::gLogManager->WriteLog("Skin is not attached to any geometry");
			}

	//		firstSkin = firstSkin->NextSibling("skin");
		}
	}

	const ColadaToPanduExporter::ColladaSkeleton* ColadaToPanduExporter::GetSkeletonHavingAllBoneSIDS(const TVertexElementDataArray& _dataArray, unsigned int _dataCount) const
	{
		const unsigned int skelCount = (unsigned int)m_Skeletons.size();

		for( unsigned int i = 0 ; i < skelCount ; i++ )
		{
			if( DoesSkeletonHaveAllBoneSIDS(*m_Skeletons[i],_dataArray,_dataCount) )
			{
				return m_Skeletons[i];
			}
		}
		return NULL;
	}

	bool ColadaToPanduExporter::DoesSkeletonHaveAllBoneSIDS(const ColladaSkeleton& _skeleton, const TVertexElementDataArray& _dataArray, unsigned int _dataCount) const
	{
		bool fewBoneDoesntExistInSkeleton = false;

		for( unsigned int dataInd = 0 ; dataInd < _dataCount ; dataInd++ )
		{
			const char* currDataBoneSID = _dataArray[dataInd].name;

			if( !DoesSkeletonContainBoneSID(_skeleton,currDataBoneSID) )
			{
				fewBoneDoesntExistInSkeleton = true;
				break;
			}
			
		}
		return !fewBoneDoesntExistInSkeleton;
	}

	bool ColadaToPanduExporter::DoesSkeletonContainBoneSID(const ColladaSkeleton& _skeleton, const char* _boneSID ) const
	{
		const unsigned int boneCount = (unsigned int)_skeleton.allBones.size();

		for( unsigned int i = 0 ; i < boneCount ; i++ )
		{
			if( strcmp(_boneSID,_skeleton.allBones[i]->sid) == 0)
			{
				return true;
			}
		}

		return false;
	}

	int ColadaToPanduExporter::GetBoneIndexOfSkeletonFromBoneSID(const ColladaSkeleton& _skeleton, const char* _boneSID ) const
	{
		const unsigned int boneCount = (unsigned int)_skeleton.allBones.size();

		for( unsigned int i = 0 ; i < boneCount ; i++ )
		{
			if( strcmp(_boneSID,_skeleton.allBones[i]->sid) == 0)
			{
				return i;
			}
		}

		return -1;
	}

	bool ColadaToPanduExporter::LoadVertexWeightsSources( const TiXmlNode* _skinNode, const TiXmlNode* _vertWeightNode,int* _oOffsets, int& _oMaxOffset, ColladaGeometry& _oGeometry ) const
	{
		const TiXmlElement* vWeightsElement = _vertWeightNode->ToElement();
		const char* vWeightsCount = vWeightsElement->Attribute("count");
		
		const TiXmlNode* inputNode = _vertWeightNode->FirstChild("input");

		_oMaxOffset = 0;

		while( inputNode )
		{
			const TiXmlElement* inputElement = inputNode->ToElement();
			const char* semantic = inputElement->Attribute("semantic");
			const char* sourceAttrib = inputElement->Attribute("source");
			const char* source = ++sourceAttrib;
			const char* offsetStr = inputElement->Attribute("offset");
			int offset = -1;

			if( offsetStr )
			{
				sscanf_s(offsetStr,"%d",&offset);

				if( offset > _oMaxOffset )
				{
					_oMaxOffset = offset;
				}
			}

			ColadaVertexDataType coladaVDataType = GetColadaVertexDataSemantic(semantic);

			if( coladaVDataType >= 0 && coladaVDataType < eCVDS_Count )
			{
				bool sourceLoaded = LoadSource( _skinNode, source,_oGeometry.elementSourceData[coladaVDataType] );
				
				if( offsetStr && sourceLoaded )
				{
					_oOffsets[coladaVDataType] = offset;
				}
			}
			else
			{
				Pandu::gLogManager->WriteLog(String("Unable to find ColadaVertexDataType named '") + semantic + "'");
			}

			inputNode = inputNode->NextSibling("input");
		}

		return true;
	}

	bool ColadaToPanduExporter::LoadSource( const TiXmlNode* _parentNode,const char* _sourceId, ElementSourceData& _oSourceData ) const
	{
		if( _oSourceData.dataCount == 0 )
		{
			const TiXmlNode* sourceNode = FindSourceNodeById(_parentNode,_sourceId);

			String sourceId;
			GetTechnique(sourceNode,sourceId,_oSourceData);

			if( _oSourceData.stride > 0 && _oSourceData.elementType >= 0 && _oSourceData.elementType < eDET_Count )
			{
				String nodeName;
				const TiXmlNode* arrayNode = GetChildNodeWithID(sourceNode,sourceId.CString(),nodeName);

				if( arrayNode )
				{
					LoadVertexSourceData(arrayNode,_oSourceData);

					return true;
				}
			}
		}
		else
		{
			Pandu::gLogManager->WriteLog(String("Source of id '") + _sourceId + "' is already loaded");
		}
		return false;
	}

	ColadaToPanduExporter::ColladaGeometry* ColadaToPanduExporter::GetColadaGeometryBasedOnId( const char* _id )
	{
		const unsigned int count = (unsigned int)m_GeometryArray.size();

		for( unsigned int i = 0 ; i < count ; i++ )
		{
			if( strcmp(_id,m_GeometryArray[i]->id) == 0 )
			{
				return m_GeometryArray[i];
			}
		}

		return NULL;
	}

	// --------------------------------------------------------
	// End export controllers functions
	// --------------------------------------------------------

	bool ColadaToPanduExporter::LoadIndices( const TiXmlNode* _parentNode, const int _indTypeOffset[eCVDS_Count], int _maxOffset, ColladaGeometry& _oGeometry ) const
	{
		const TiXmlElement* triangleElement = _parentNode->ToElement();
		std::stringstream triangleCountStrm(triangleElement->Attribute("count"));

		const TiXmlNode* vcountNode = _parentNode->FirstChild("vcount");
		const TiXmlElement* vcountNodeElement = vcountNode->ToElement();
		const char* text = vcountNodeElement->GetText();
		std::stringstream stringStream(text);

		while( !stringStream.eof() )
		{
			int data = 0;
			stringStream >> data;

			for( unsigned int i = 0 ; i < eCVDS_Count ; i++ )
			{
				if( _indTypeOffset[i] >= 0 )
				{
					_oGeometry.vcountIndices[i].push_back(data);
				}
			}
		}

		const TiXmlNode* pNode = _parentNode->FirstChild("v");
		//not required the while loop in open collada, its required only in 3ds max collada file
		while( pNode )
		{
			const TiXmlElement* pNodeElement = pNode->ToElement();
			const char* text = pNodeElement->GetText();

			std::stringstream stringStream(text);

			unsigned int count = 0;
			const unsigned int totalOffset = _maxOffset + 1;

			while( !stringStream.eof() )
			{
				int data = 0;
				stringStream >> data;

				for( int i = 0 ; i < eCVDS_Count ; i++ )
				{
					if( _indTypeOffset[i] >= 0 )
					{
						//const int currOffset = _indTypeOffset[i] + 1;
						if( (count % totalOffset) == _indTypeOffset[i] )
						{
							_oGeometry.vertexIndices[i].push_back(data);
						}
					}
				}
				count++;
			}

			pNode = pNode->FirstChild("p");
		}

		return true;
	}

	const TiXmlNode* ColadaToPanduExporter::GetChildNodeWithID( const TiXmlNode* _parentNode, const char* const _id, String& _oNodeName) const
	{
		const TiXmlNode* child = _parentNode->FirstChild();
		
		while( child )
		{
			const TiXmlElement* inputElement = child->ToElement();
			const char* id = inputElement->Attribute("id");

			if( strcmp(id,_id) == 0 )
			{
				_oNodeName = child->Value();
				return child;
			}

			child = child->NextSibling();
		}

		return NULL;
	}

	// --------------------------------------------------------
	// Start Geometry Conversion functions
	// --------------------------------------------------------

	void ColadaToPanduExporter::ConverAllGeometriesToPANDUGeometryBinary()
	{
		unsigned int count = (unsigned int)m_GeometryArray.size();

		String tempExportPath = m_ExportPath;

		if( tempExportPath.CString()[m_ExportPath.Length()-1] != '/' && tempExportPath.CString()[m_ExportPath.Length()-1] != '\\' )
		{
			tempExportPath = tempExportPath + "/";
		}

		for( unsigned int i = 0 ; i < count ; i++ )
		{
			ColladaGeometry* coladaGeometry = m_GeometryArray[i];
			GeometryBinary* newGeometryBinary = new GeometryBinary(tempExportPath + coladaGeometry->name);// + String(".") + (char*)&GeometryBinary::gGeometryBinaryMagicNumber);
			bool converted = ConvertGeometryToPANDUGeometryBinary(*coladaGeometry,*newGeometryBinary);
			if( converted )
			{
				BinaryDataFile::BFileHeaderChunk &fileHeader = newGeometryBinary->GetFileHeader();
				BinaryDataFile::BFileDataChunk &geometry = newGeometryBinary->GetDataChunk();

				newGeometryBinary->SetDataLoaded();
				m_GeometryBinaryArray.push_back(newGeometryBinary);
			}
			else
			{
				delete newGeometryBinary;
			}
		}
	}

	bool ColadaToPanduExporter::ConvertGeometryToPANDUGeometryBinary(const ColladaGeometry& _coladaGeometry, GeometryBinary& _oGeometryBinary)
	{
		BinaryDataFile::BFileHeaderChunk &fileHeader = _oGeometryBinary.GetFileHeader();
		BinaryDataFile::BFileDataChunk &geometry = _oGeometryBinary.GetDataChunk();
		BinaryDataFile::BFileData &fileData = _oGeometryBinary.GetFileData();

		if( strlen(_coladaGeometry.name) >= MAX_NAME_SIZE )
		{
			assert(false);
			Pandu::gLogManager->WriteLog(String("ERROR - Name has more than ") + (MAX_NAME_SIZE-1) + " characters, can't convert " + _coladaGeometry.name + " to PANDUGeometryBinary");
			return false;
		}

		BinaryDataFile::BFileData::DataBlock* newDataBlock = fileData.AddNewDataBlock();

		const unsigned int dataSize = strlen(_coladaGeometry.name) + 1;
		newDataBlock->m_DataBlockHeader.m_DataBlockSemantic = GeometryBinary::eGBS_MESHNAME;
		newDataBlock->m_DataBlockHeader.m_DataElementType = BinaryDataFile::eBDET_Char;
		newDataBlock->m_DataBlockHeader.m_DataElementSize = kGBDataElementTypeSizes[BinaryDataFile::eBDET_Char];
		newDataBlock->m_DataBlockHeader.m_DataStride = 1;
		newDataBlock->m_DataBlockHeader.m_DataCount = dataSize;
		newDataBlock->m_Data = static_cast<char*>(std::malloc(dataSize));
		strcpy_s(newDataBlock->m_Data,dataSize,_coladaGeometry.name);
		
		geometry.AddSemantic(GeometryBinary::eGBS_MESHNAME);


		unsigned int oPositionSemanticCount= 0;
		for( unsigned int i = 0 ; i < ColadaToPanduExporter::eCVDS_Count ; i++ )
		{
			bool posSemFound = false;
			const ElementSourceData& elemSourceData = _coladaGeometry.elementSourceData[i];
			if( elemSourceData.dataCount > 0 && (elemSourceData.dataCount % elemSourceData.stride) == 0 )
			{
				ConverElementSourceToPANDUGeometryBinary(_coladaGeometry,elemSourceData,(ColadaVertexDataType)i,_oGeometryBinary,posSemFound);
				if( posSemFound )
				{
					oPositionSemanticCount++;
				}
			}
		}


		if( oPositionSemanticCount > 0 )
		{
			if( oPositionSemanticCount > 1 )
			{
				Pandu::gLogManager->WriteLog(String("There is more than one position array found in colada geometry ") + _coladaGeometry.name + "...");
				return false;
			}
			else
			{
				const unsigned int count = (unsigned int)_coladaGeometry.vertexIndices[eCVDS_Position].size();
				bool indsLoaded = false;

				if( count > 0 && (count % 3) == 0 )
				{
					BinaryDataFile::BFileData::DataBlock* indicesDataBlock = fileData.AddNewDataBlock();

					indicesDataBlock->m_DataBlockHeader.m_DataBlockSemantic = GeometryBinary::eGBS_INDICES;
					indicesDataBlock->m_DataBlockHeader.m_DataElementType = BinaryDataFile::eBDET_UnsignedInt;
					indicesDataBlock->m_DataBlockHeader.m_DataElementSize = kGBDataElementTypeSizes[BinaryDataFile::eBDET_UnsignedInt];
					indicesDataBlock->m_DataBlockHeader.m_DataStride = 1;
					indicesDataBlock->m_DataBlockHeader.m_DataCount = count;

					const unsigned int dataSize = count * kGBDataElementTypeSizes[BinaryDataFile::eBDET_UnsignedInt];

					indicesDataBlock->m_Data = static_cast<char*>(std::malloc(dataSize));

					memcpy(indicesDataBlock->m_Data,(const void*)&(_coladaGeometry.vertexIndices[eCVDS_Position])[0],dataSize);
					
					geometry.AddSemantic(GeometryBinary::eGBS_INDICES);

					indsLoaded = true;
				}
				else
				{
					Pandu::gLogManager->WriteLog(String("Indices count is not multiple of 3 in Colada Geometry ") + _coladaGeometry.name + "...");
					return false;
				}

				const unsigned int boneInfluenceCountIndsCount = (unsigned int)_coladaGeometry.vcountIndices[eCVDS_Weight].size();
				const unsigned int boneWeightsCount = (unsigned int)_coladaGeometry.vertexIndices[eCVDS_Weight].size();
				const unsigned int boneIndicesCount = (unsigned int)_coladaGeometry.vertexIndices[eCVDS_Joint].size();

				if( indsLoaded && boneInfluenceCountIndsCount > 0 && boneWeightsCount && boneIndicesCount > 0)
				{
					//----------------------------------------------------------------
					//Indices count data block
					{
						BinaryDataFile::BFileData::DataBlock* indicesCountDataBlock = fileData.AddNewDataBlock();

						indicesCountDataBlock->m_DataBlockHeader.m_DataBlockSemantic = GeometryBinary::eGBS_BONEINFLUENCECOUNTINDICES;
						indicesCountDataBlock->m_DataBlockHeader.m_DataElementType = BinaryDataFile::eBDET_UnsignedInt;
						indicesCountDataBlock->m_DataBlockHeader.m_DataElementSize = kGBDataElementTypeSizes[BinaryDataFile::eBDET_UnsignedInt];
						indicesCountDataBlock->m_DataBlockHeader.m_DataStride = 1;
						indicesCountDataBlock->m_DataBlockHeader.m_DataCount = boneInfluenceCountIndsCount;

						unsigned int dataSize = boneInfluenceCountIndsCount * kGBDataElementTypeSizes[BinaryDataFile::eBDET_UnsignedInt];

						indicesCountDataBlock->m_Data = static_cast<char*>(std::malloc(dataSize));

						memcpy(indicesCountDataBlock->m_Data,(const void*)&(_coladaGeometry.vcountIndices[eCVDS_Weight])[0],dataSize);

						geometry.AddSemantic(GeometryBinary::eGBS_BONEINFLUENCECOUNTINDICES);
					}
					//----------------------------------------------------------------
					//bone weights data block
					{
						BinaryDataFile::BFileData::DataBlock* boneWeightsDataBlock = fileData.AddNewDataBlock();

						boneWeightsDataBlock->m_DataBlockHeader.m_DataBlockSemantic = GeometryBinary::eGBS_WEIGHTINDICES;
						boneWeightsDataBlock->m_DataBlockHeader.m_DataElementType = BinaryDataFile::eBDET_UnsignedInt;
						boneWeightsDataBlock->m_DataBlockHeader.m_DataElementSize = kGBDataElementTypeSizes[BinaryDataFile::eBDET_UnsignedInt];
						boneWeightsDataBlock->m_DataBlockHeader.m_DataStride = 1;
						boneWeightsDataBlock->m_DataBlockHeader.m_DataCount = boneWeightsCount;

						unsigned int dataSize = boneWeightsCount * kGBDataElementTypeSizes[BinaryDataFile::eBDET_UnsignedInt];

						boneWeightsDataBlock->m_Data = static_cast<char*>(std::malloc(dataSize));

						memcpy(boneWeightsDataBlock->m_Data,(const void*)&(_coladaGeometry.vertexIndices[eCVDS_Weight])[0],dataSize);

						geometry.AddSemantic(GeometryBinary::eGBS_WEIGHTINDICES);
					}

					//bone weights data block
					{
						BinaryDataFile::BFileData::DataBlock* vertBoneInfluenceDataBlock = fileData.AddNewDataBlock();

						vertBoneInfluenceDataBlock->m_DataBlockHeader.m_DataBlockSemantic = GeometryBinary::eGBS_BONEINDICES;
						vertBoneInfluenceDataBlock->m_DataBlockHeader.m_DataElementType = BinaryDataFile::eBDET_UnsignedInt;
						vertBoneInfluenceDataBlock->m_DataBlockHeader.m_DataElementSize = kGBDataElementTypeSizes[BinaryDataFile::eBDET_UnsignedInt];
						vertBoneInfluenceDataBlock->m_DataBlockHeader.m_DataStride = 1;
						vertBoneInfluenceDataBlock->m_DataBlockHeader.m_DataCount = boneIndicesCount;

						unsigned int dataSize = boneIndicesCount * kGBDataElementTypeSizes[BinaryDataFile::eBDET_UnsignedInt];

						vertBoneInfluenceDataBlock->m_Data = static_cast<char*>(std::malloc(dataSize));

						memcpy(vertBoneInfluenceDataBlock->m_Data,(const void*)&(_coladaGeometry.vertexIndices[eCVDS_Joint])[0],dataSize);

						geometry.AddSemantic(GeometryBinary::eGBS_BONEINDICES);
					}

					return true;
				}
				else
				{
					return indsLoaded;
				}
			}
		}
		else
		{
			Pandu::gLogManager->WriteLog(String("There is no position found in Colada Geometry ") + _coladaGeometry.name + "...");
			return false;
		}

		return false;
	}

	void ColadaToPanduExporter::ConverElementSourceToPANDUGeometryBinary(const ColladaGeometry& _colladaGeometry, const ElementSourceData& _elementSourceData, ColadaVertexDataType _vDataType, GeometryBinary& _oGeometryBinary, bool& _oPositionSemanticFound)
	{
		if( (_elementSourceData.dataCount >= _elementSourceData.stride) && (_elementSourceData.dataCount % _elementSourceData.stride) == 0)
		{
			GeometryBinary::GBSemantic gbSemantic = kGBSemanticBasedOnColadaVertexDataType[_vDataType];
			BinaryDataFile::BDataElementType gbDataElementType = kGBDataElementTypeBasedOnDataElementType[_elementSourceData.elementType];

			if( _colladaGeometry.skeleton && gbSemantic == GeometryBinary::eGBS_SKELETONNAME && (unsigned int)_elementSourceData.elementDataArray.size() > 0 )
			{
				gbDataElementType = BinaryDataFile::eBDET_Char;

				BinaryDataFile::BFileDataChunk &geometry = _oGeometryBinary.GetDataChunk();
				BinaryDataFile::BFileData &fileData = _oGeometryBinary.GetFileData();

				BinaryDataFile::BFileData::DataBlock* newDataBlock = fileData.AddNewDataBlock();
				
				const unsigned dataCount = strlen(_elementSourceData.elementDataArray[0].name) + 1;

				newDataBlock->m_DataBlockHeader.m_DataBlockSemantic = gbSemantic;
				newDataBlock->m_DataBlockHeader.m_DataElementType = gbDataElementType;
				newDataBlock->m_DataBlockHeader.m_DataElementSize = kGBDataElementTypeSizes[gbDataElementType];
				newDataBlock->m_DataBlockHeader.m_DataStride = 1;
				newDataBlock->m_DataBlockHeader.m_DataCount = dataCount;
				
				const unsigned int memSize = dataCount;
				newDataBlock->m_Data = static_cast<char*>(std::malloc(memSize));

				memcpy(newDataBlock->m_Data,&_elementSourceData.elementDataArray[0],memSize);
				
				geometry.AddSemantic(gbSemantic);
			}
			else if(_colladaGeometry.skeleton && gbSemantic == GeometryBinary::eGBS_BONES && (unsigned int)_elementSourceData.elementDataArray.size() > 0 )
			{
				gbDataElementType = BinaryDataFile::eBDET_UnsignedInt;

				BinaryDataFile::BFileDataChunk &geometry = _oGeometryBinary.GetDataChunk();
				BinaryDataFile::BFileData &fileData = _oGeometryBinary.GetFileData();

				BinaryDataFile::BFileData::DataBlock* newDataBlock = fileData.AddNewDataBlock();

				newDataBlock->m_DataBlockHeader.m_DataBlockSemantic = gbSemantic;
				newDataBlock->m_DataBlockHeader.m_DataElementType = gbDataElementType;
				newDataBlock->m_DataBlockHeader.m_DataElementSize = kGBDataElementTypeSizes[gbDataElementType];
				newDataBlock->m_DataBlockHeader.m_DataStride = 1;
				newDataBlock->m_DataBlockHeader.m_DataCount = _elementSourceData.dataCount;

				const unsigned int memSize = kGBDataElementTypeSizes[gbDataElementType] * _elementSourceData.dataCount;
				newDataBlock->m_Data = static_cast<char*>(std::malloc(memSize));

				unsigned int* boneInds = (unsigned int*)newDataBlock->m_Data;

				for( unsigned int i = 0 ; i < _elementSourceData.dataCount ; i++ )
				{
					int boneId = GetBoneIndexOfSkeletonFromBoneSID(*_colladaGeometry.skeleton,_elementSourceData.elementDataArray[i].name);
					if( boneId < 0 )
					{
						Pandu::gLogManager->WriteLog(String("Skeleton doesn't contain bone with SID '") + _elementSourceData.elementDataArray[i].name + "'...");
					}
					boneInds[i] = boneId;
				}

				geometry.AddSemantic(gbSemantic);
			}
			else if( gbSemantic != GeometryBinary::eGBS_None && gbDataElementType < BinaryDataFile::eBDET_Count )
			{
				BinaryDataFile::BFileDataChunk &geometry = _oGeometryBinary.GetDataChunk();
				BinaryDataFile::BFileData &fileData = _oGeometryBinary.GetFileData();

				BinaryDataFile::BFileData::DataBlock* newDataBlock = fileData.AddNewDataBlock();
				
				newDataBlock->m_DataBlockHeader.m_DataBlockSemantic = gbSemantic;
				newDataBlock->m_DataBlockHeader.m_DataElementType = gbDataElementType;
				newDataBlock->m_DataBlockHeader.m_DataElementSize = kGBDataElementTypeSizes[gbDataElementType];
				newDataBlock->m_DataBlockHeader.m_DataStride = _elementSourceData.stride;
				newDataBlock->m_DataBlockHeader.m_DataCount = _elementSourceData.dataCount;
				
				const unsigned int memSize = _elementSourceData.dataCount * kGBDataElementTypeSizes[gbDataElementType];
				newDataBlock->m_Data = static_cast<char*>(std::malloc(memSize));

				memcpy(newDataBlock->m_Data,&_elementSourceData.elementDataArray[0],memSize);
				
				geometry.AddSemantic(gbSemantic);

				if( gbSemantic == GeometryBinary::eGBS_POSITION )
				{
					_oPositionSemanticFound = true;
				}
				else
				{
					_oPositionSemanticFound = false;
				}
			}
			else
			{
				Pandu::gLogManager->WriteLog("Unsupported semantic...");
			}	
		}
		else
		{
			assert(false);
			Pandu::gLogManager->WriteLog("ERROR - Data count is not multiple of stride...");
		}
	}

	void ColadaToPanduExporter::DumpAllGeometryBinaryToFile() const
	{
		unsigned int count = (unsigned int)m_GeometryBinaryArray.size();

		for( unsigned int i = 0 ; i < count ; i++ )
		{
			m_GeometryBinaryArray[i]->DumpDataToFile();
		}
	}

	//skeleton conversion
	void ColadaToPanduExporter::ConvertAllSkeletonsToPANDUSkeletonBinary()
	{
		const unsigned int skelCount = (unsigned int)m_Skeletons.size();

		String tempExportPath = m_ExportPath;

		if( tempExportPath.CString()[m_ExportPath.Length()-1] != '/' && tempExportPath.CString()[m_ExportPath.Length()-1] != '\\' )
		{
			tempExportPath = tempExportPath + "/Skeleton/";
		}
		else
		{
			tempExportPath = tempExportPath + "Skeleton/";
		}


		for( unsigned int i = 0 ; i < skelCount ; i++ )
		{
			ColladaSkeleton* skeletonGeometry = m_Skeletons[i];
			SkeletonBinary* newSkeletonBinary = new SkeletonBinary(tempExportPath + skeletonGeometry->name);// + String(".") + (char*)&GeometryBinary::gGeometryBinaryMagicNumber);
			bool converted = ConvertSkeletonToPANDUSkeletonBinary(*skeletonGeometry,*newSkeletonBinary);
			if( converted )
			{
				BinaryDataFile::BFileHeaderChunk &fileHeader = newSkeletonBinary->GetFileHeader();
				BinaryDataFile::BFileDataChunk &skeleton = newSkeletonBinary->GetDataChunk();

				newSkeletonBinary->SetDataLoaded();
				m_SkeletonBinary.push_back(newSkeletonBinary);
			}
			else
			{
				delete newSkeletonBinary;
			}
		}
	}

	bool ColadaToPanduExporter::ConvertSkeletonToPANDUSkeletonBinary(const ColladaSkeleton& _colladaSkeleton, SkeletonBinary& _oSkeletonBinary)
	{
		BinaryDataFile::BFileHeaderChunk &fileHeader = _oSkeletonBinary.GetFileHeader();
		BinaryDataFile::BFileDataChunk &skeleton = _oSkeletonBinary.GetDataChunk();
		BinaryDataFile::BFileData &fileData = _oSkeletonBinary.GetFileData();

		if( strlen(_colladaSkeleton.name) >= MAX_NAME_SIZE )
		{
			assert(false);
			Pandu::gLogManager->WriteLog(String("ERROR - Name has more than ") + (MAX_NAME_SIZE-1) + " characters, can't convert " + _colladaSkeleton.name + " to PANDUSkeletonBinary");
			return false;
		}

		BinaryDataFile::BFileData::DataBlock* newDataBlock = fileData.AddNewDataBlock();

		const unsigned int dataCount = strlen(_colladaSkeleton.name) + 1;

		newDataBlock->m_DataBlockHeader.m_DataBlockSemantic = SkeletonBinary::eSBS_SKELETONNAME;
		newDataBlock->m_DataBlockHeader.m_DataElementType = BinaryDataFile::eBDET_Char;
		newDataBlock->m_DataBlockHeader.m_DataElementSize = kSBDataElementTypeSizes[BinaryDataFile::eBDET_Char];
		newDataBlock->m_DataBlockHeader.m_DataStride = 1;
		newDataBlock->m_DataBlockHeader.m_DataCount = dataCount;
		newDataBlock->m_Data = static_cast<char*>(std::malloc(dataCount));
		strcpy_s(newDataBlock->m_Data,dataCount,_colladaSkeleton.name);
		
		skeleton.AddSemantic(SkeletonBinary::eSBS_SKELETONNAME);

		if( ConverBonesNameIdToPANDUSkeletonBinary(_colladaSkeleton,true,_oSkeletonBinary) )
		{
			if( ConverBonesNameIdToPANDUSkeletonBinary(_colladaSkeleton,false,_oSkeletonBinary) )
			{
				if( ConverTransformsToPANDUSkeletonBinary(_colladaSkeleton,_oSkeletonBinary) )
				{
					unsigned int oTotalChildCount = 0;
					if( ConverChildCountToPANDUSkeletonBinary(_colladaSkeleton,_oSkeletonBinary,oTotalChildCount) )
					{
						if( ConvertChildsToPANDUSkeletonBinary(_colladaSkeleton,_oSkeletonBinary,oTotalChildCount) )
						{
							return true;
						}
					}
				}
			}
		}

		return false;
	}

	//childs
	bool ColadaToPanduExporter::ConvertChildsToPANDUSkeletonBinary( const ColladaSkeleton& _colladaSkeleton, SkeletonBinary& _oSkeletonBinary, unsigned int _totalChildCount )
	{
		
		BinaryDataFile::BFileDataChunk &skeleton = _oSkeletonBinary.GetDataChunk();
		BinaryDataFile::BFileData &fileData = _oSkeletonBinary.GetFileData();

		
		BinaryDataFile::BFileData::DataBlock* newDataBlock = fileData.AddNewDataBlock();
		
		newDataBlock->m_DataBlockHeader.m_DataBlockSemantic = SkeletonBinary::eSBS_CHILD;
		newDataBlock->m_DataBlockHeader.m_DataElementType = BinaryDataFile::eBDET_UnsignedInt;
		newDataBlock->m_DataBlockHeader.m_DataElementSize = kSBDataElementTypeSizes[BinaryDataFile::eBDET_UnsignedInt];
		newDataBlock->m_DataBlockHeader.m_DataStride = 1;
		newDataBlock->m_DataBlockHeader.m_DataCount = _totalChildCount;
		newDataBlock->m_Data = static_cast<char*>(std::malloc(_totalChildCount * kSBDataElementTypeSizes[BinaryDataFile::eBDET_UnsignedInt]));
		
		unsigned int *uintData = (unsigned int*)newDataBlock->m_Data;

		const unsigned int bonesCount = (unsigned int)_colladaSkeleton.allBones.size();
		unsigned int child = 0;
		for( unsigned int i = 0 ; i < bonesCount ; i++ )
		{
			const unsigned int currChildCount = (unsigned int)_colladaSkeleton.allBones[i]->childs.size();

			for( unsigned int j = 0 ; j < currChildCount ; j++ )
			{
				uintData[child] = _colladaSkeleton.allBones[i]->childs[j]->index;
				child++;
			}
		}
		
		skeleton.AddSemantic(SkeletonBinary::eSBS_CHILD);

		return true;

	}

	//childcounts
	bool ColadaToPanduExporter::ConverChildCountToPANDUSkeletonBinary( const ColladaSkeleton& _colladaSkeleton, SkeletonBinary& _oSkeletonBinary, unsigned int& _oTotalChildCount)
	{
		BinaryDataFile::BFileDataChunk &skeleton = _oSkeletonBinary.GetDataChunk();
		BinaryDataFile::BFileData &fileData = _oSkeletonBinary.GetFileData();

		const unsigned int bonesCount = (unsigned int)_colladaSkeleton.allBones.size();

		BinaryDataFile::BFileData::DataBlock* newDataBlock = fileData.AddNewDataBlock();
		
		newDataBlock->m_DataBlockHeader.m_DataBlockSemantic = SkeletonBinary::eSBS_CHILDCOUNT;
		newDataBlock->m_DataBlockHeader.m_DataElementType = BinaryDataFile::eBDET_UnsignedInt;
		newDataBlock->m_DataBlockHeader.m_DataElementSize = kSBDataElementTypeSizes[BinaryDataFile::eBDET_UnsignedInt];
		newDataBlock->m_DataBlockHeader.m_DataStride = 1;
		newDataBlock->m_DataBlockHeader.m_DataCount = bonesCount;
		newDataBlock->m_Data = static_cast<char*>(std::malloc(bonesCount * kSBDataElementTypeSizes[BinaryDataFile::eBDET_UnsignedInt]));
		
		unsigned int *uintData = (unsigned int*)newDataBlock->m_Data;

		_oTotalChildCount = 0;
		for( unsigned int i = 0 ; i < bonesCount ; i++ )
		{
			unsigned int currChildCount = (unsigned int)_colladaSkeleton.allBones[i]->childs.size();
			uintData[i] = currChildCount;
			_oTotalChildCount += currChildCount;
		}
		
		skeleton.AddSemantic(SkeletonBinary::eSBS_CHILDCOUNT);

		return true;
	}

	//Transforms
	bool ColadaToPanduExporter::ConverTransformsToPANDUSkeletonBinary( const ColladaSkeleton& _colladaSkeleton, SkeletonBinary& _oSkeletonBinary)
	{
		BinaryDataFile::BFileDataChunk &skeleton = _oSkeletonBinary.GetDataChunk();
		BinaryDataFile::BFileData &fileData = _oSkeletonBinary.GetFileData();

		const unsigned int bonesCount = (unsigned int)_colladaSkeleton.allBones.size();
		const unsigned int dataCount = bonesCount * 16; //4x4 matrix

		BinaryDataFile::BFileData::DataBlock* newDataBlock = fileData.AddNewDataBlock();
		
		newDataBlock->m_DataBlockHeader.m_DataBlockSemantic = SkeletonBinary::eSBS_TRANSFORM;
		newDataBlock->m_DataBlockHeader.m_DataElementType = BinaryDataFile::eBDET_Float4x4;
		newDataBlock->m_DataBlockHeader.m_DataElementSize = kSBDataElementTypeSizes[BinaryDataFile::eBDET_Float4x4];
		newDataBlock->m_DataBlockHeader.m_DataStride = 16;
		newDataBlock->m_DataBlockHeader.m_DataCount = dataCount;
		newDataBlock->m_Data = static_cast<char*>(std::malloc(dataCount * kSBDataElementTypeSizes[BinaryDataFile::eBDET_Float4x4]));
		
		float *uintData = (float*)newDataBlock->m_Data;

		unsigned int floatCount = 0;
		for( unsigned int i = 0 ; i < bonesCount ; i++ )
		{
			for( unsigned int j = 0 ; j < 16 ; j++ )
			{
				uintData[floatCount++] = _colladaSkeleton.allBones[i]->transform.arr[j];
			}
		}
		
		skeleton.AddSemantic(SkeletonBinary::eSBS_TRANSFORM);

		return true;
	}

	//names
	bool ColadaToPanduExporter::ConverBonesNameIdToPANDUSkeletonBinary( const ColladaSkeleton& _colladaSkeleton, bool boneID, SkeletonBinary& _oSkeletonBinary)
	{
		BinaryDataFile::BFileDataChunk &skeleton = _oSkeletonBinary.GetDataChunk();
		BinaryDataFile::BFileData &fileData = _oSkeletonBinary.GetFileData();

		const unsigned int bonesCount = (unsigned int)_colladaSkeleton.allBones.size();

		if( bonesCount > 0 )
		{
			BinaryDataFile::BFileData::DataBlock* newDataBlock = fileData.AddNewDataBlock();
			String boneIdString;
			char seperator[2] = {'\n','\0'};
			seperator[0] = BinaryDataFile::gNameSeperator;
			
			for( unsigned int i = 0 ; i < bonesCount ; i++ )
			{
				Bone* bone = _colladaSkeleton.allBones[i];
				if(boneID)
				{
					boneIdString = boneIdString + bone->id;
				}
				else
				{
					boneIdString = boneIdString + bone->name;
				}
				if( i < (bonesCount - 1) )
				{
					boneIdString = boneIdString + seperator;
				}
			}

			const unsigned int size = boneIdString.StdString().size();
			const unsigned int dataSize = size + 1;

			if(boneID)
			{
				newDataBlock->m_DataBlockHeader.m_DataBlockSemantic = SkeletonBinary::eSBS_BONEID;
			}
			else
			{
				newDataBlock->m_DataBlockHeader.m_DataBlockSemantic = SkeletonBinary::eSBS_BONENAME;
			}
			newDataBlock->m_DataBlockHeader.m_DataElementType = BinaryDataFile::eBDET_Name;
			newDataBlock->m_DataBlockHeader.m_DataElementSize = kSBDataElementTypeSizes[BinaryDataFile::eBDET_Name];
			newDataBlock->m_DataBlockHeader.m_DataStride = 1;
			newDataBlock->m_DataBlockHeader.m_DataCount = dataSize;
			newDataBlock->m_Data = static_cast<char*>(std::malloc(dataSize));
			memcpy(newDataBlock->m_Data,boneIdString.CString(),dataSize);
			
			if(boneID)
			{
				skeleton.AddSemantic(SkeletonBinary::eSBS_BONEID);
			}
			else
			{
				skeleton.AddSemantic(SkeletonBinary::eSBS_BONENAME);
			}

			return true;
		}

		return false;
	}

	void ColadaToPanduExporter::DumpAllSkeletonsBinaryToFile() const
	{
		unsigned int count = (unsigned int)m_SkeletonBinary.size();

		for( unsigned int i = 0 ; i < count ; i++ )
		{
			m_SkeletonBinary[i]->DumpDataToFile();
		}
	}

	// --------------------------------------------------------
	// End Geometry Conversion functions
	// --------------------------------------------------------

	// --------------------------------------------------------
	// Start export skeleton functions
	// --------------------------------------------------------
	void ColadaToPanduExporter::LoadAllSkeleton( const TiXmlNode* _coladaNode )
	{
		const TiXmlNode* libraryScenes = _coladaNode->FirstChild("library_visual_scenes");

		if( libraryScenes )
		{
			const TiXmlNode* firstVisualScene = libraryScenes->FirstChild("visual_scene");

			if( firstVisualScene )
			{
				//while( firstVisualScene )
				{
					//need to check later
					//if( HasSkeletonInstanceControllerNode( firstVisualScene ) )
					{
						ColladaSkeleton* newSkeleton = new ColladaSkeleton();
						if( LoadSkeleton( firstVisualScene, *newSkeleton ) )
						{
							if( newSkeleton->root && (int)newSkeleton->allBones.size() > 0 )
							{
								m_Skeletons.push_back(newSkeleton);
							}
							else
							{
								delete newSkeleton;
								Pandu::gLogManager->WriteLog("ERROR - Unable to load skeleton");
							}
						}
						else
						{
							Pandu::gLogManager->WriteLog("ERROR - Unable to load skeleton");
						}
					}
					//firstVisualScene = firstVisualScene->NextSibling("visual_scene");
				}
			}
		}
	}

	bool ColadaToPanduExporter::LoadSkeleton( const TiXmlNode* _visualScene, ColladaSkeleton& _oSkeleton) const
	{
		const TiXmlElement* visualSceneELement = _visualScene->ToElement();
		const char* skeletonName = visualSceneELement->Attribute("name");
		strcpy_s(_oSkeleton.name,MAX_NAME_SIZE,skeletonName);

		const TiXmlNode* rootJoint = GetNextNodeOfType(_visualScene,NULL,"JOINT");

		if( rootJoint )
		{
			const TiXmlElement* nodeElement = rootJoint->ToElement();
			const char* id = nodeElement->Attribute("id");
			const char* name = nodeElement->Attribute("name");
			const char* sid = nodeElement->Attribute("sid");

			Bone* rootBone = new Bone();
			strcpy_s(rootBone->id,MAX_NAME_SIZE,id);
			strcpy_s(rootBone->name,MAX_NAME_SIZE,name);

			if( sid )
			{
				strcpy_s(rootBone->sid,MAX_NAME_SIZE,sid);
			}
			else
			{
				strcpy_s(rootBone->sid,MAX_NAME_SIZE,id);
			}
			
			
			rootBone->parent = NULL;
			rootBone->index = (unsigned int)_oSkeleton.allBones.size();
			_oSkeleton.allBones.push_back(rootBone);
			_oSkeleton.root = rootBone;

			LoadAllChilds(rootJoint,rootBone,_oSkeleton);

			return true;
		}

		return false;
	}

	void ColadaToPanduExporter::LoadAllChilds( const TiXmlNode* _parent, Bone* _parentBone, ColladaSkeleton& _oSkeleton ) const
	{
		LoadTransformForNode(_parent,_parentBone->transform);
		//const Vector3 pos = _parentBone->transform.GetTranslate();

		const TiXmlNode* child = GetNextNodeOfType(_parent,NULL,"JOINT");

		while(child)
		{
			const TiXmlElement* nodeElement = child->ToElement();
			const char* id = nodeElement->Attribute("id");
			const char* name = nodeElement->Attribute("name");
			const char* sid = nodeElement->Attribute("sid");

			Bone* childBone = FindBoneWithId(_oSkeleton,id);
			
			if( !childBone )
			{
				childBone = new Bone();
				strcpy_s(childBone->id,MAX_NAME_SIZE,id);
				strcpy_s(childBone->name,MAX_NAME_SIZE,name);
				if( sid )
				{
					strcpy_s(childBone->sid,MAX_NAME_SIZE,sid);
				}
				else
				{
					strcpy_s(childBone->sid,MAX_NAME_SIZE,id);
				}
				
				childBone->index = (unsigned int)_oSkeleton.allBones.size();
				_oSkeleton.allBones.push_back(childBone);
			}
			assert(!childBone->parent);
			childBone->parent = _parentBone;
			_parentBone->childs.push_back(childBone);
			LoadAllChilds(child,childBone,_oSkeleton);

			child = GetNextNodeOfType(_parent,child,"JOINT") ;
		}
	}

	bool ColadaToPanduExporter::LoadTransformForNode( const TiXmlNode* _node, Matrix44& _transform ) const
	{
		const TiXmlNode* child = _node->FirstChild();
		const char* childName = child->Value();
		bool matrixFound = false;
		bool matrixIsTransform = false;

		_transform = Matrix44::ZERO;
		
		Vector3 translate = Vector3::ZERO;
		Quaternion rotate = Quaternion::IDENTITY;

		while( child && strcmp(childName,"node") != 0 && strcmp(childName,"extra") != 0 )
		{
			const TiXmlElement* nodeElement = child->ToElement();
			const char* dataText = nodeElement->GetText();

			if( strcmp(childName,"matrix") == 0 )
			{
				matrixFound = true;
				std::stringstream stringStream(dataText);
				int dataCount = 0;
			
				while( !stringStream.eof() && dataCount < 16 )
				{
					float data = 0;
					stringStream >> data;
					_transform.arr[dataCount++] = data;
				}
				matrixIsTransform = true;
				break;
			}
			else if( strcmp(childName,"translate") == 0 )
			{
				matrixFound = true;
				sscanf_s(dataText,"%f %f %f",&translate.x,&translate.y,&translate.z);
			}
			else if( strcmp(childName,"rotate") == 0 )
			{
				matrixFound = true;
				float angleDeg;
				Vector3 axis;
				sscanf_s(dataText,"%f %f %f %f",&axis.x,&axis.y,&axis.z,&angleDeg);

				Quaternion localRot(GetRadiansFromDegrees(angleDeg),axis);
				rotate = localRot * rotate;
			}

			child = child->NextSibling();
			childName = child->Value();
		}

		if( !matrixIsTransform )
		{
			_transform.MakeTransform(translate,rotate);
		}

		return matrixFound;
	}

	ColadaToPanduExporter::Bone* ColadaToPanduExporter::FindBoneWithId(const ColladaSkeleton& _oSkeleton, const char* const _id ) const
	{
		const unsigned int boneCount = (unsigned int)_oSkeleton.allBones.size();

		for( unsigned int i = 0 ; i < boneCount ; i++ )
		{
			Bone* bone = _oSkeleton.allBones[i];
			if( strcmp(bone->id,_id) == 0 )
			{
				return bone;
			}
		}
		return NULL;
	}

	const TiXmlNode* ColadaToPanduExporter::GetNextNodeOfType( const TiXmlNode* _parent, const TiXmlNode* _currNode, const char* const _type ) const
	{
		const TiXmlNode* node = _currNode;
		
		if( !node )
		{
			node = _parent->FirstChild("node");
		}
		else
		{
			node = node->NextSibling("node");
		}

		while( node )
		{
			const TiXmlElement* nodeElement = node->ToElement();
			const char* nodeType = nodeElement->Attribute("type");

			if( nodeType && strcmp(nodeType,_type) == 0 )
			{
				return node;
			}
		
			node = node->NextSibling("node");
		}

		return NULL;
	}

	// --------------------------------------------------------
	// End export skeleton functions
	// --------------------------------------------------------

	// --------------------------------------------------------
	// Start export animation
	// --------------------------------------------------------
	void ColadaToPanduExporter::LoadAllAnimations( const TiXmlNode* _coladaNode )
	{
		const TiXmlNode* libraryAnimations = _coladaNode->FirstChild("library_animations");

		if( libraryAnimations )
		{
			const TiXmlNode* firstAnimation = libraryAnimations->FirstChild("animation");
			while(firstAnimation)
			{
				LoadAnimation(firstAnimation);
				firstAnimation = firstAnimation->NextSibling("animation");
			}
		}
	}

	void ColadaToPanduExporter::LoadAnimation( const TiXmlNode* _animation)
	{
		const TiXmlNode* channelNode = _animation->FirstChild("channel");

		if( channelNode )
		{
			const TiXmlElement* element = channelNode->ToElement();
			const char* sourceElm = element->Attribute("source");
			if( sourceElm )
			{
				sourceElm++;

				const char* targetElm = element->Attribute("target");

				if( targetElm )
				{
					StringTokenizer tokenizer(targetElm,'/');

					if( tokenizer.HasNextToken() )
					{
						const String boneId = tokenizer.GetNextToken();
						if( tokenizer.HasNextToken() )
						{
							const String transformType = tokenizer.GetNextToken();
							AnimationTransformType animTransType = GetAnimationTransformType(transformType.CString());
							if( animTransType >= 0 && animTransType < eATT_Count )
							{
								ColladaSkeletonAnimationClip* clip = FindSkeletonAnimClipForBone(boneId.CString());
								strcpy_s(clip->name,MAX_NAME_SIZE,"TestAnim");//for temporary purpose
								if(clip)
								{
									if(clip->HasAnimationForBoneId(boneId.CString()))
									{
										Pandu::gLogManager->WriteLog(String("Clip already has animation for bone '" + boneId + "'"));
									}
									else
									{
										const TiXmlNode* sampler = FindSamplerSourceById(_animation,sourceElm);

										if(sampler)
										{
											if( LoadSourcesFromSampler(boneId.CString(),animTransType,_animation,sampler,*clip))
											{
												return;
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}

		Pandu::gLogManager->WriteLog(String("Unable to load animation"));
	}

	bool ColadaToPanduExporter::LoadSourcesFromSampler(const char* boneId, AnimationTransformType _tt, const TiXmlNode* _animNode, const TiXmlNode* _samplerNode, ColladaSkeletonAnimationClip& _oSklAnimClip) const
	{
		const TiXmlNode* input = _samplerNode->FirstChild("input");
		bool inputLoaded = false;
		bool outputLoaded = false;
		bool interpolationLoaded = false;

		const Bone* bone = FindBoneWithId(*_oSklAnimClip.skeleton,boneId);
		ColladaSkeletonAnimationNode* animNode = _oSklAnimClip.AddAnimationNode(boneId,_tt,bone->index);

		while(input)
		{
			const TiXmlElement* inputElement = input->ToElement();
			const char* semantic = inputElement->Attribute("semantic");
			if( !inputLoaded && strcmp(semantic,"INPUT") == 0 )
			{
				const char* source = inputElement->Attribute("source");
				if( source )
				{
					source++;
					if(!LoadSource(_animNode,source,animNode->inputSourceData))
					{
						return false;
					}
				}
				else
				{
					return false;
				}
			}
			else if( !outputLoaded && strcmp(semantic,"OUTPUT") == 0 )
			{
				const char* source = inputElement->Attribute("source");
				if( source )
				{
					source++;
					if(!LoadSource(_animNode,source,animNode->outputSourceData))
					{
						return false;
					}
				}
				else
				{
					return false;
				}
			}
			else if( !interpolationLoaded && strcmp(semantic,"INTERPOLATION") == 0 )
			{
				const char* source = inputElement->Attribute("source");
				if( source )
				{
					source++;
					if(!LoadSource(_animNode,source,animNode->interpolationData))
					{
						return false;
					}
				}
				else
				{
					return false;
				}
			}

			input = input->NextSibling("input");
		}

		return true;
	}

	ColadaToPanduExporter::AnimationTransformType ColadaToPanduExporter::GetAnimationTransformType(const char* _charTransformType) const
	{
		for( unsigned int i = 0 ; i < eATT_Count ; i++ )
		{
			if( strcmp(kAnimationTransformType[i],_charTransformType) == 0 )
			{
				return (AnimationTransformType)i;
			}
		}

		return eATT_Count;
	}

	ColadaToPanduExporter::ColladaSkeletonAnimationClip* ColadaToPanduExporter::FindSkeletonAnimClipForBone(const char* _boneId) 
	{
		const unsigned int sklCount = (unsigned int)m_Skeletons.size();

		for( unsigned int i = 0 ; i < sklCount ; i++ )
		{
			ColladaSkeleton* skeleton = m_Skeletons[i];
			Bone* bone = FindBoneWithId(*skeleton,_boneId );

			if( bone )
			{
				ColladaSkeletonAnimationClip* clip = FindAnimationClipForSkeleton(skeleton->name,NULL);
				if( !clip )
				{
					clip = new ColladaSkeletonAnimationClip();
					clip->skeleton = skeleton;
					m_ColladaAnimClips.push_back(clip);
				}
				return clip;
			}
		}
		return NULL;
	}

	ColadaToPanduExporter::ColladaSkeletonAnimationClip* ColadaToPanduExporter::FindAnimationClipForSkeleton(const char* _sklName, const char* _clipName) const
	{
		const unsigned int clipCount = (unsigned int)m_ColladaAnimClips.size();

		for( unsigned int i = 0 ; i < clipCount ; i++ )
		{
			ColladaSkeletonAnimationClip* currClip = m_ColladaAnimClips[i];
			if( strcmp(currClip->skeleton->name,_sklName) == 0 )
			{
				if( !_clipName || (_clipName && strcmp(currClip->name,_clipName) == 0 ) )
				{
					return currClip;
				}
			}
		}

		return NULL;
	}

	const TiXmlNode* ColadaToPanduExporter::FindSamplerSourceById(const TiXmlNode* _animNode, const char* _samplerSourceId) const
	{
		const TiXmlNode* samplerNode = _animNode->FirstChild("sampler");

		while(samplerNode)
		{
			const TiXmlElement* samplerElement = samplerNode->ToElement();
			const char* id = samplerElement->Attribute("id");

			if( strcmp( id, _samplerSourceId ) == 0 )
			{
				return samplerNode;
			}

			samplerNode = samplerNode->NextSibling("sampler");
		}
		return NULL;
	}

	void ColadaToPanduExporter::ConvertAllAnimationToPANDUAnimationBinary()
	{
		const unsigned int skelCount = (unsigned int)m_Skeletons.size();

		String tempExportPath = m_ExportPath;

		if( tempExportPath.CString()[m_ExportPath.Length()-1] != '/' && tempExportPath.CString()[m_ExportPath.Length()-1] != '\\' )
		{
			tempExportPath = tempExportPath + "/Animation/";
		}
		else
		{
			tempExportPath = tempExportPath + "Animation/";
		}


		const unsigned int animCount = (unsigned int)m_ColladaAnimClips.size();

		for( unsigned int i = 0 ; i < animCount ; i++ )
		{
			ColladaSkeletonAnimationClip* animClip = m_ColladaAnimClips[i];
			AnimationBinary* newAnimBinary = new AnimationBinary(tempExportPath + animClip->name);// + String(".") + (char*)&GeometryBinary::gGeometryBinaryMagicNumber);
			bool converted = ConvertAnimationToPANDUAnimationBinary(*animClip,*newAnimBinary);
			if( converted )
			{
				BinaryDataFile::BFileHeaderChunk &fileHeader = newAnimBinary->GetFileHeader();
				BinaryDataFile::BFileDataChunk &skeleton = newAnimBinary->GetDataChunk();

				newAnimBinary->SetDataLoaded();
				m_AnimationBinary.push_back(newAnimBinary);
			}
			else
			{
				delete newAnimBinary;
			}

		}
	}

	bool ColadaToPanduExporter::ConvertAnimationToPANDUAnimationBinary(const ColladaSkeletonAnimationClip& _colladaAnim, AnimationBinary& _oAnimBinary)
	{
		BinaryDataFile::BFileHeaderChunk &fileHeader = _oAnimBinary.GetFileHeader();
		BinaryDataFile::BFileDataChunk &skeleton = _oAnimBinary.GetDataChunk();
		BinaryDataFile::BFileData &fileData = _oAnimBinary.GetFileData();

		if( strlen(_colladaAnim.name) >= MAX_NAME_SIZE )
		{
			assert(false);
			Pandu::gLogManager->WriteLog(String("ERROR - Name has more than ") + (MAX_NAME_SIZE-1) + " characters, can't convert " + _colladaAnim.name + " to AnimationBinary");
			return false;
		}

		// -------------------------------------------------------------------
		// SkeletonName

		BinaryDataFile::BFileData::DataBlock* sklNameDataBlock = fileData.AddNewDataBlock();

		const unsigned int dataCount = strlen(_colladaAnim.skeleton->name) + 1;

		sklNameDataBlock->m_DataBlockHeader.m_DataBlockSemantic = AnimationBinary::eABS_SKELETON_NAME;
		sklNameDataBlock->m_DataBlockHeader.m_DataElementType = BinaryDataFile::eBDET_Char;
		sklNameDataBlock->m_DataBlockHeader.m_DataElementSize = kSBDataElementTypeSizes[BinaryDataFile::eBDET_Char];
		sklNameDataBlock->m_DataBlockHeader.m_DataStride = 1;
		sklNameDataBlock->m_DataBlockHeader.m_DataCount = dataCount;
		sklNameDataBlock->m_Data = static_cast<char*>(std::malloc(dataCount));
		strcpy_s(sklNameDataBlock->m_Data,dataCount,_colladaAnim.skeleton->name);

		skeleton.AddSemantic(AnimationBinary::eABS_SKELETON_NAME);

		// -------------------------------------------------------------------
		// AnimationName

		BinaryDataFile::BFileData::DataBlock* animNameDataBlock = fileData.AddNewDataBlock();

		const unsigned int animNameDataCount = strlen(_colladaAnim.name) + 1;

		animNameDataBlock->m_DataBlockHeader.m_DataBlockSemantic = AnimationBinary::eABS_ANIM_NAME;
		animNameDataBlock->m_DataBlockHeader.m_DataElementType = BinaryDataFile::eBDET_Char;
		animNameDataBlock->m_DataBlockHeader.m_DataElementSize = kSBDataElementTypeSizes[BinaryDataFile::eBDET_Char];
		animNameDataBlock->m_DataBlockHeader.m_DataStride = 1;
		animNameDataBlock->m_DataBlockHeader.m_DataCount = animNameDataCount;
		animNameDataBlock->m_Data = static_cast<char*>(std::malloc(animNameDataCount));
		strcpy_s(animNameDataBlock->m_Data,animNameDataCount,_colladaAnim.name);

		skeleton.AddSemantic(AnimationBinary::eABS_ANIM_NAME);

		// -------------------------------------------------------------------
		// Animation Node

		TBoneNameAnimationNodeMap::const_iterator itr = _colladaAnim.animationNodes.begin();
		unsigned int animationNodesExported = 0;
	
		while( itr != _colladaAnim.animationNodes.end() )
		{
			const std::pair<const String,ColladaSkeletonAnimationNode*>& boneNameAnimNodePair = (*itr);
			const ColladaSkeletonAnimationNode* animNode = boneNameAnimNodePair.second;
			bool converted = ConvertAnimationNodeToPanduAnimationBinary(boneNameAnimNodePair.first,*animNode,_oAnimBinary);

			if( !converted )
			{
				Pandu::gLogManager->WriteLog("Unable to export animation node");
			}
			else
			{
				animationNodesExported++;
			}
			itr++;
		}

		return (animationNodesExported > 0);
	}

	bool ColadaToPanduExporter::ConvertAnimationNodeToPanduAnimationBinary(const String& _boneName, const ColladaSkeletonAnimationNode& _coladaAnimNode, AnimationBinary& _oAnimBinary)
	{
		BinaryDataFile::BFileHeaderChunk &fileHeader = _oAnimBinary.GetFileHeader();
		BinaryDataFile::BFileDataChunk &skeleton = _oAnimBinary.GetDataChunk();
		BinaryDataFile::BFileData &fileData = _oAnimBinary.GetFileData();

		// convert input source
		{
			const ElementSourceData& sourceData = _coladaAnimNode.inputSourceData;
			BinaryDataFile::BFileData::DataBlock* boneAnimDataInput = fileData.AddNewDataBlock();

			const unsigned int dataCount = sourceData.dataCount;
			const unsigned int dataElementSize = kSBDataElementTypeSizes[sourceData.elementType];
			const unsigned int dataSize = dataCount * dataElementSize;

			boneAnimDataInput->m_DataBlockHeader.m_DataBlockSemantic = AnimationBinary::eABS_BONE_ANIM_TIME_DATA;
			boneAnimDataInput->m_DataBlockHeader.m_DataElementType = sourceData.elementType;
			boneAnimDataInput->m_DataBlockHeader.m_DataElementSize = dataElementSize;
			boneAnimDataInput->m_DataBlockHeader.m_DataStride = sourceData.stride;
			boneAnimDataInput->m_DataBlockHeader.m_DataCount = dataCount;
			boneAnimDataInput->m_Data = static_cast<char*>(std::malloc(dataSize));
			boneAnimDataInput->SetUserData(0,_coladaAnimNode.boneIndex);//user data 0 is used for bone index;

			memcpy(boneAnimDataInput->m_Data,(const void*)(&sourceData.elementDataArray[0]),dataSize);
			
			skeleton.AddSemantic(AnimationBinary::eABS_BONE_ANIM_TIME_DATA);
		}

		// convert output source
		{
			const ElementSourceData& sourceData = _coladaAnimNode.outputSourceData;
			BinaryDataFile::BFileData::DataBlock* boneAnimDataOutput = fileData.AddNewDataBlock();

			const unsigned int dataCount = sourceData.dataCount;
			const unsigned int dataElementSize = kSBDataElementTypeSizes[sourceData.elementType];
			const unsigned int dataSize = dataCount * dataElementSize;

			boneAnimDataOutput->m_DataBlockHeader.m_DataBlockSemantic = AnimationBinary::eABS_BONE_ANIM_TRANSFORM_DATA;
			boneAnimDataOutput->m_DataBlockHeader.m_DataElementType = sourceData.elementType;
			boneAnimDataOutput->m_DataBlockHeader.m_DataElementSize = dataElementSize;
			boneAnimDataOutput->m_DataBlockHeader.m_DataStride = sourceData.stride;
			boneAnimDataOutput->m_DataBlockHeader.m_DataCount = dataCount;
			boneAnimDataOutput->m_Data = static_cast<char*>(std::malloc(dataSize));
			boneAnimDataOutput->SetUserData(0,_coladaAnimNode.boneIndex);//user data 0 is used for bone index;

			memcpy(boneAnimDataOutput->m_Data,(const void*)(&sourceData.elementDataArray[0]),dataSize);

			skeleton.AddSemantic(AnimationBinary::eABS_BONE_ANIM_TRANSFORM_DATA);
		}

		// convert interpolations
		{
			const ElementSourceData& sourceData = _coladaAnimNode.interpolationData;
			BinaryDataFile::BFileData::DataBlock* boneAnimDataOutput = fileData.AddNewDataBlock();

			const unsigned int dataCount = sourceData.dataCount;
			const unsigned int dataElementSize = kSBDataElementTypeSizes[eDET_UnsignedInt];
			const unsigned int dataSize = dataCount * dataElementSize;

			boneAnimDataOutput->m_DataBlockHeader.m_DataBlockSemantic = AnimationBinary::eABS_BONE_ANIM_INTERPOLATE_DATA;
			boneAnimDataOutput->m_DataBlockHeader.m_DataElementType = eDET_UnsignedInt;
			boneAnimDataOutput->m_DataBlockHeader.m_DataElementSize = dataElementSize;
			boneAnimDataOutput->m_DataBlockHeader.m_DataStride = 1;
			boneAnimDataOutput->m_DataBlockHeader.m_DataCount = dataCount;
			boneAnimDataOutput->m_Data = static_cast<char*>(std::malloc(dataSize));
			boneAnimDataOutput->SetUserData(0,_coladaAnimNode.boneIndex);//user data 0 is used for bone index;

			unsigned int* dataPtr = (unsigned int*)(boneAnimDataOutput->m_Data);

			for( unsigned int i = 0 ; i < dataCount ; i++ )
			{
				dataPtr[i] = Interpolator::eIM_Linear;//only linear supported
			}

			skeleton.AddSemantic(AnimationBinary::eABS_BONE_ANIM_INTERPOLATE_DATA);
		}

		return true;
	}

	void ColadaToPanduExporter::DumpAllAnimationBinaryToFile() const
	{
		const unsigned int animCount = (unsigned int)m_AnimationBinary.size();

		for( unsigned int i = 0 ; i < animCount ; i++ )
		{
			m_AnimationBinary[i]->DumpDataToFile();
		}
	}
}