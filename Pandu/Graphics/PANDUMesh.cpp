#include "PANDUMesh.h"
#include <boost/assert.hpp>
#include "PANDUGlInclude.h"


namespace
{
	static const unsigned int kGLPolygonModes[Pandu::Mesh::ePM_Count] = 
											{
												GL_LINES,			//ePM_Lines
												GL_LINE_LOOP,		//ePM_LineLoop
												GL_LINE_STRIP,		//ePM_LineStrip
												GL_TRIANGLES,		//ePM_Triangles
												GL_TRIANGLE_STRIP,	//ePM_TriangleStrip
												GL_TRIANGLE_FAN,	//ePM_TriangleFan
											};

}

namespace Pandu
{
	//===============================================================================
	//Mesh Implementation

	class Mesh::Impl
	{
	private:

		friend class Mesh;

		TMeshInstancesList		m_InstancesList;

		const String			m_MeshName;
		String					m_SkeletonName;

		Mesh*					m_Mesh;
		VertexBuffer			m_VertexBuffer;
		VertexBuffer			m_ModifiedVertexBuffer;

		TInvBindMatrixArray		m_InverseBindMatrices;
		Matrix44				m_BindShapeMatrix;

		bool					m_Locked;
		Mesh::PolygonMode		m_PolygonMode;


		//===============================================================================
		//MeshInstanceImpl Implementation

		class MeshInstanceImpl : public MeshInstance
		{
		private:

			Mesh* m_Mesh;
			TMeshModifiersSharedPtrList m_ModifiersList;

			void UpdateModifiers()
			{
				TMeshModifiersSharedPtrList::iterator itr = m_ModifiersList.begin();
				TMeshModifiersSharedPtrList::iterator end = m_ModifiersList.end();

				for( ; itr != end ; itr++ )
				{
					(*itr)->ModifyMesh(m_Mesh->m_Impl->GetVertexBuffer(),m_Mesh->m_Impl->GetModifiedVertexBuffer());
				}
			}

			virtual void Render(const Matrix44& _transform, unsigned int _techniqueID, bool _useFixedFunctionIfNoTechniqueAvailable)
			{
				float matrix[16];

				int p = 0;
				for(int i = 0 ; i < 4 ; i++)
				{
					for(int j = 0 ; j < 4 ; j++)
					{
						matrix[p] = _transform[j][i];
						p++;
					}
				}

				glPushMatrix();
				glMultMatrixf(matrix);

				UpdateModifiers();
				Material* currMat = &Material::defaultMaterial;
				if( m_Material != NULL )
				{
					currMat = (m_Material.get());
				}

				currMat->EnableMaterialProperties();

				ShaderEffect* effect = currMat->GetShaderEffect();

				if(effect)
				{
					TSharedShaderTechniquePtr currTechnique = effect->GetTechnique(_techniqueID);
					if(currTechnique)
					{
						const unsigned short passCount = currTechnique->GetPassCount();
						for( unsigned short i = 0 ; i < passCount ; i++ )
						{
							TSharedShaderPassPtr pass = currTechnique->GetPass(i);
							if( pass != NULL )
							{
								RenderUsingPass(_transform, pass.get());
							}
						}
					}
					else if(_useFixedFunctionIfNoTechniqueAvailable)
					{
						m_Mesh->m_Impl->Render();
					}
				}
				else if(_useFixedFunctionIfNoTechniqueAvailable)
				{
					m_Mesh->m_Impl->Render();
				}

				currMat->DisableMaterialProperties();

				glPopMatrix();
			}

			void RenderUsingPass(const Matrix44& _transform, ShaderPass* _pass)
			{
				_pass->SetAutoShaderParameterValue(AutoShaderParameter::ASPT_ModelMatrix,_transform);

				static const unsigned short end = AutoShaderParameter::ASPT_ModelShadowProjectionMatrix0 + AutoShaderParameter::GetShadowProjectionMatrixParamCount();

				for( unsigned int i = AutoShaderParameter::ASPT_ModelShadowProjectionMatrix0 ; i < end ; i++ )
				{
					const AutoShaderParameter::AutoShaderParameterType type = (AutoShaderParameter::AutoShaderParameterType)i;
					const Matrix44 modelShadowProjMat = AutoShaderParameter::GetShadowProjectionMatrix(type) * _transform;
					_pass->SetAutoShaderParameterValue(type,modelShadowProjMat);
				}

				_pass->BeginPass();

				std::vector<TSharedShaderAttributePtr> enabledAttrib;

				for( unsigned short i = 0 ; i < VertexBuffer::gBufferCountCanCreateAttribute ; i++ )
				{
					const VertexBuffer::BufferType buffType = VertexBuffer::gBufferTypeCanCreateAttribute[i];
					Buffer* buffer = m_Mesh->m_Impl->GetBufferToRender( buffType );
					if( buffer && buffer->GetTotalDataCount() > 0 )
					{
						TSharedShaderAttributePtr attrb = _pass->GetShaderAttributeForBuffer( buffType );
						if( attrb != NULL )
						{
							attrb->EnableAttribute();
							attrb->SetValue3((float*)buffer->GetData());
							enabledAttrib.push_back(attrb);
						}
					}
				}

				m_Mesh->m_Impl->Render();
				
				
				const unsigned int count = (unsigned int)enabledAttrib.size();
				for(unsigned int i = 0  ; i < count ; i++ )
				{
					enabledAttrib[i]->DisableAttribute();
					enabledAttrib[i].reset();
				}

				_pass->EndPass();
			}

			void ClearModifiers()
			{
				TMeshModifiersSharedPtrList::iterator itr = m_ModifiersList.begin();
				TMeshModifiersSharedPtrList::iterator end = m_ModifiersList.end();

				for( ; itr != end ; itr++ )
				{
					PANDU_ERROR((*itr).use_count() == 1, "Modifier is still in use please clear all the references" );

					(*itr).reset();
				}

				m_ModifiersList.clear();
			}

			void AddModifier(TMeshModifierAutoPtr _modifier)
			{
				_modifier->SetMesh(m_Mesh);
				m_ModifiersList.push_back(TMeshModifierSharedPtr(_modifier.release()));
			}


		public:

			MeshInstanceImpl(Mesh* _mesh)
				: MeshInstance()
				, m_Mesh(_mesh)
			{

			}

			~MeshInstanceImpl()
			{
				ClearModifiers();
				m_Mesh = NULL;
			}

		};

	public:

		Impl(const String& _meshName,Mesh* _mesh)
			: m_MeshName(_meshName)
			, m_Mesh(_mesh)
			, m_BindShapeMatrix(Matrix44::IDENTITY)
			, m_Locked(false)
			, m_PolygonMode(Mesh::ePM_Triangles)
		{
			
		}

		~Impl()
		{
			Destroy();
		}

		void Destroy()
		{
			m_InverseBindMatrices.clear();
		}

		void ClearBuffer(VertexBuffer& _buffer, VertexBuffer::BufferType _bt)
		{
			_buffer.FreeBuffer(_bt);
		}

		void ClearMeshInstances(VertexBuffer& _buffer)
		{
			TMeshInstancesList::iterator itr = m_InstancesList.begin();
			TMeshInstancesList::iterator end = m_InstancesList.end();

			for( ; itr != end ; itr++ )
			{
				PANDU_ERROR((*itr).use_count() == 1, (String("Mesh Instance named ") + m_Mesh->GetName()
					+ " is still in use, first clear the reference before clear.." ).CString() );

				(*itr).reset();
			}

			m_InstancesList.clear();
		}

		inline const String& GetName() const
		{
			return m_MeshName;
		}

		bool SetVertexBuffer(const float* const _vBuffer, unsigned int _vCount)
		{
			PANDU_ERROR(m_Locked,"Before modifying mesh first lock it and after finishing unlock it");

			if(m_Locked)
			{
				ClearBuffer(m_VertexBuffer,VertexBuffer::eBT_Vertex);
				m_VertexBuffer.AllocateBuffer(VertexBuffer::eBT_Vertex,Buffer::eBET_Float,_vCount,3,sizeof(float));
				m_VertexBuffer.CopyDataToBuffer(VertexBuffer::eBT_Vertex,(char*)_vBuffer,sizeof(float) * _vCount);
				return true;
			}
			return false;
		}

		bool SetIndexBuffer(const unsigned int* const _iBuffer, unsigned int _iCount)
		{
			PANDU_ERROR(m_Locked,"Before modifying mesh first lock it and after finishing unlock it");

			if(m_Locked)
			{
				ClearBuffer(m_VertexBuffer,VertexBuffer::eBT_Index);
				m_VertexBuffer.AllocateBuffer(VertexBuffer::eBT_Index,Buffer::eBET_UnsignedInt,_iCount,1,sizeof(unsigned int));
				m_VertexBuffer.CopyDataToBuffer(VertexBuffer::eBT_Index,(char*)_iBuffer,sizeof(unsigned int) * _iCount);
				return true;
			}
			return false;
		}

		bool SetTexCoordBuffer(const float* const _tBuffer, unsigned int _tCount)
		{
			PANDU_ERROR(m_Locked,"Before modifying mesh first lock it and after finishing unlock it");

			if(m_Locked)
			{
				ClearBuffer(m_VertexBuffer,VertexBuffer::eBT_Texcoord0);
				m_VertexBuffer.AllocateBuffer(VertexBuffer::eBT_Texcoord0,Buffer::eBET_Float,_tCount,2,sizeof(float));
				m_VertexBuffer.CopyDataToBuffer(VertexBuffer::eBT_Texcoord0,(char*)_tBuffer,sizeof(float) * _tCount);
				return true;
			}
			return false;
		}

		bool SetNormalBuffer(const float* const _nBuffer, unsigned int _nCount)
		{
			PANDU_ERROR(m_Locked,"Before modifying mesh first lock it and after finishing unlock it");

			if(m_Locked)
			{
				ClearBuffer(m_VertexBuffer,VertexBuffer::eBT_Normal);
				m_VertexBuffer.AllocateBuffer(VertexBuffer::eBT_Normal,Buffer::eBET_Float,_nCount,3,sizeof(float));
				m_VertexBuffer.CopyDataToBuffer(VertexBuffer::eBT_Normal,(char*)_nBuffer,sizeof(float) * _nCount);
				return true;
			}
			return false;
		}

		bool SetVertexBufferWithDataOwnership(float* _vBuffer, unsigned int _vCount)
		{
			PANDU_ERROR(m_Locked,"Before modifying mesh first lock it and after finishing unlock it");

			if(m_Locked)
			{
				ClearBuffer(m_VertexBuffer,VertexBuffer::eBT_Vertex);
				m_VertexBuffer.SetDataWithOwnership(VertexBuffer::eBT_Vertex,(char*)_vBuffer,Buffer::eBET_Float,_vCount,3,sizeof(float));
				return true;
			}
			return false;
		}

		bool SetIndexBufferWithDataOwnership(unsigned int* _iBuffer, unsigned int _iCount)
		{
			PANDU_ERROR(m_Locked,"Before modifying mesh first lock it and after finishing unlock it");

			if(m_Locked)
			{
				ClearBuffer(m_VertexBuffer,VertexBuffer::eBT_Index);
				m_VertexBuffer.SetDataWithOwnership(VertexBuffer::eBT_Index,(char*)_iBuffer,Buffer::eBET_UnsignedInt,_iCount,1,sizeof(unsigned int));
				return true;
			}
			return false;
		}

		bool SetTexCoordBufferWithDataOwnership(float* _tBuffer, unsigned int _tCount)
		{
			PANDU_ERROR(m_Locked,"Before modifying mesh first lock it and after finishing unlock it");

			if(m_Locked)
			{
				ClearBuffer(m_VertexBuffer,VertexBuffer::eBT_Texcoord0);
				m_VertexBuffer.SetDataWithOwnership(VertexBuffer::eBT_Texcoord0,(char*)_tBuffer,Buffer::eBET_Float,_tCount,2,sizeof(float));
				return true;
			}
			return false;
		}

		bool SetNormalBufferWithDataOwnership(float* _nBuffer, unsigned int _nCount)
		{
			PANDU_ERROR(m_Locked,"Before modifying mesh first lock it and after finishing unlock it");

			if(m_Locked)
			{
				ClearBuffer(m_VertexBuffer,VertexBuffer::eBT_Normal);
				m_VertexBuffer.SetDataWithOwnership(VertexBuffer::eBT_Normal,(char*)_nBuffer,Buffer::eBET_Float,_nCount,3,sizeof(float));
				return true;
			}
			return false;
		}

		bool SetTangentBufferWithDataOwnership(float* _tgntBuffer, unsigned int _tgntCount)
		{
			PANDU_ERROR(m_Locked,"Before modifying mesh first lock it and after finishing unlock it");

			if(m_Locked)
			{
				ClearBuffer(m_VertexBuffer,VertexBuffer::eBT_Tangent);
				m_VertexBuffer.SetDataWithOwnership(VertexBuffer::eBT_Tangent,(char*)_tgntBuffer,Buffer::eBET_Float,_tgntCount,3,sizeof(float));
				return true;
			}
			return false;
		}

		bool SetBinormalBufferWithDataOwnership(float* _biBuffer, unsigned int _biCount )
		{
			PANDU_ERROR(m_Locked,"Before modifying mesh first lock it and after finishing unlock it");

			if(m_Locked)
			{
				ClearBuffer(m_VertexBuffer,VertexBuffer::eBT_Binormal);
				m_VertexBuffer.SetDataWithOwnership(VertexBuffer::eBT_Binormal,(char*)_biBuffer,Buffer::eBET_Float,_biCount,3,sizeof(float));
				return true;
			}
			return false;
		}

		bool SetBonesWithDataOwnership(unsigned int* _boneIndBuffer, unsigned int _boneCount)
		{
			PANDU_ERROR(m_Locked,"Before modifying mesh first lock it and after finishing unlock it");

			if(m_Locked)
			{
				ClearBuffer(m_VertexBuffer,VertexBuffer::eBT_Bones);
				m_VertexBuffer.SetDataWithOwnership(VertexBuffer::eBT_Bones,(char*)_boneIndBuffer,Buffer::eBET_UnsignedInt,_boneCount,2,sizeof(unsigned int));
				return true;
			}
			return false;
		}

		bool SetVertexWeightBufferWithDataOwnership(float* _weightBuffer, unsigned int _weightCount)
		{
			PANDU_ERROR(m_Locked,"Before modifying mesh first lock it and after finishing unlock it");

			if(m_Locked)
			{
				ClearBuffer(m_VertexBuffer,VertexBuffer::eBT_VertexWeight);
				m_VertexBuffer.SetDataWithOwnership(VertexBuffer::eBT_VertexWeight,(char*)_weightBuffer,Buffer::eBET_Float,_weightCount,2,sizeof(float));
				return true;
			}
			return false;
		}

		bool SetBoneInfluenceCountIndexBufferWithDataOwnership(unsigned int* _boneInfCountIndsBuffer, unsigned int _count)
		{
			PANDU_ERROR(m_Locked,"Before modifying mesh first lock it and after finishing unlock it");

			if(m_Locked)
			{
				ClearBuffer(m_VertexBuffer,VertexBuffer::eBT_BoneInfluenceCountIndex);
				m_VertexBuffer.SetDataWithOwnership(VertexBuffer::eBT_BoneInfluenceCountIndex,(char*)_boneInfCountIndsBuffer,Buffer::eBET_UnsignedInt,_count,1,sizeof(unsigned int));
				return true;
			}
			return false;
		}

		bool SetBoneIndexBufferWithDataOwnership(unsigned int* _boneIndexBuffer, unsigned int _count)
		{
			PANDU_ERROR(m_Locked,"Before modifying mesh first lock it and after finishing unlock it");

			if(m_Locked)
			{
				ClearBuffer(m_VertexBuffer,VertexBuffer::eBT_BoneIndex);
				m_VertexBuffer.SetDataWithOwnership(VertexBuffer::eBT_BoneIndex,(char*)_boneIndexBuffer,Buffer::eBET_UnsignedInt,_count,1,sizeof(unsigned int));
				return true;
			}
			return false;
		}

		bool SetVertexWeightIndexBufferWithDataOwnership(unsigned int* _weightIndexBuffer, unsigned int _count)
		{
			PANDU_ERROR(m_Locked,"Before modifying mesh first lock it and after finishing unlock it");

			if(m_Locked)
			{
				ClearBuffer(m_VertexBuffer,VertexBuffer::eBT_VertexWeightIndex);
				m_VertexBuffer.SetDataWithOwnership(VertexBuffer::eBT_VertexWeightIndex,(char*)_weightIndexBuffer,Buffer::eBET_UnsignedInt,_count,1,sizeof(unsigned int));
				return true;
			}
			return false;
		}

		void AddInverseBindMatrix(const Matrix44& _matrix)
		{
			const unsigned int matCount = (unsigned int)m_InverseBindMatrices.size();
			const unsigned int boneCount = m_VertexBuffer.GetBuffer(VertexBuffer::eBT_Bones)->GetTotalDataCount();

			PANDU_ERROR( matCount < boneCount,"Already Inv bind matrices filled up");
			
			if( matCount < boneCount)
			{
				m_InverseBindMatrices.push_back(_matrix);
			}
		}

		void SetBindShapeMatrix(const Matrix44& _matrix)
		{
			m_BindShapeMatrix = _matrix;
		}

		inline void Lock()
		{
			m_Locked = true;
		}

		inline void Unlock()
		{
			m_Locked = false;
		}

		void Render()
		{
			if(!m_Locked)
			{
				Buffer* normalBuffer = GetBufferToRender(VertexBuffer::eBT_Normal);
				if(normalBuffer && normalBuffer->GetTotalDataCount() > 0 && normalBuffer->GetData())
				{
					glEnableClientState(GL_NORMAL_ARRAY);
					glNormalPointer(GL_FLOAT,0,normalBuffer->GetData());
				}
				else
				{
					glDisableClientState(GL_NORMAL_ARRAY);
				}

				Buffer* texCoord0Buffer = GetBufferToRender(VertexBuffer::eBT_Texcoord0);
				if(texCoord0Buffer && texCoord0Buffer->GetTotalDataCount() > 0 && texCoord0Buffer->GetData() )
				{
					glEnableClientState(GL_TEXTURE_COORD_ARRAY);
					glTexCoordPointer(texCoord0Buffer->GetDataStride(),GL_FLOAT,0,texCoord0Buffer->GetData());
				}
				else
				{
					glDisableClientState(GL_TEXTURE_COORD_ARRAY);
				}

				Buffer* vertBuffer = GetBufferToRender(VertexBuffer::eBT_Vertex);

				glEnableClientState(GL_VERTEX_ARRAY);
				glVertexPointer(vertBuffer->GetDataStride(), GL_FLOAT, 0, vertBuffer->GetData());

				Buffer* indBuffer = GetBufferToRender(VertexBuffer::eBT_Index);
				glDrawElements(kGLPolygonModes[m_PolygonMode], indBuffer->GetTotalDataCount(),GL_UNSIGNED_INT, indBuffer->GetData());
			}
		}

		Buffer* GetBufferToRender(VertexBuffer::BufferType _bt)
		{
			Buffer* originalBuff = m_VertexBuffer.GetBuffer(_bt);
			Buffer* modBuff = m_ModifiedVertexBuffer.GetBuffer(_bt);

			return (modBuff?modBuff:originalBuff);
		}

		TSharedMeshInstancePtr CreateMeshInstance()
		{
			TSharedMeshInstancePtr meshInstance(new MeshInstanceImpl(m_Mesh));
			m_InstancesList.push_back(meshInstance);
			return meshInstance;
		}


		inline const VertexBuffer& GetVertexBuffer() const
		{
			return m_VertexBuffer;
		}

		inline VertexBuffer& GetModifiedVertexBuffer()
		{
			return m_ModifiedVertexBuffer;
		}
		
		inline const TInvBindMatrixArray& GetInvBindMatricesArray() const
		{
			return m_InverseBindMatrices;
		}
		
		inline const Matrix44& GetBindShapeMatrix() const
		{
			return m_BindShapeMatrix;
		}

		inline void SetPolygonMode(PolygonMode _polygonMode)
		{
			m_PolygonMode = _polygonMode;
		}
	};

	

	//===============================================================================

	Mesh::Mesh(const String& _meshName)
	{
		m_Impl.reset(new Impl(_meshName,this));
	}
	
	Mesh::~Mesh()
	{
	}

	const String& Mesh::GetName() const
	{
		return m_Impl->GetName();
	}

	bool Mesh::SetVertexBuffer(const float* const _vBuffer, unsigned int _vCount)
	{
		return m_Impl->SetVertexBuffer(_vBuffer,_vCount);
	}

	bool Mesh::SetIndexBuffer(const unsigned int* const _iBuffer, unsigned int _iCount)
	{
		return m_Impl->SetIndexBuffer(_iBuffer,_iCount);
	}

	bool Mesh::SetTexCoordBuffer(const float* const _tBuffer, unsigned int _tCount)
	{
		return m_Impl->SetTexCoordBuffer(_tBuffer,_tCount);
	}

	bool Mesh::SetNormalBuffer(const float* const _nBuffer, unsigned int _nCount)
	{
		return m_Impl->SetNormalBuffer(_nBuffer,_nCount);
	}

	bool Mesh::SetVertexBufferWithDataOwnership(float* _vBuffer, unsigned int _vCount)
	{
		return m_Impl->SetVertexBufferWithDataOwnership( _vBuffer, _vCount);
	}

	bool Mesh::SetIndexBufferWithDataOwnership(unsigned int* _iBuffer, unsigned int _iCount)
	{
		return m_Impl->SetIndexBufferWithDataOwnership( _iBuffer, _iCount);
	}

	bool Mesh::SetTexCoordBufferWithDataOwnership(float* _tBuffer, unsigned int _tCount)
	{
		return m_Impl->SetTexCoordBufferWithDataOwnership( _tBuffer, _tCount);
	}

	bool Mesh::SetNormalBufferWithDataOwnership(float* _nBuffer, unsigned int _nCount)
	{
		return m_Impl->SetNormalBufferWithDataOwnership( _nBuffer, _nCount);
	}

	bool Mesh::SetTangentBufferWithDataOwnership(float* _tgntBuffer, unsigned int _tgntCount)
	{
		return m_Impl->SetTangentBufferWithDataOwnership(_tgntBuffer,_tgntCount);
	}

	bool Mesh::SetBinormalBufferWithDataOwnership(float* _biBuffer, unsigned int _biCount )
	{
		return m_Impl->SetBinormalBufferWithDataOwnership(_biBuffer,_biCount);
	}

	void Mesh::Lock()
	{
		m_Impl->Lock();
	}

	void Mesh::Unlock()
	{
		m_Impl->Unlock();
	}

	TSharedMeshInstancePtr Mesh::CreateMeshInstance() const
	{
		return m_Impl->CreateMeshInstance();
	}

	void Mesh::SetSkeletonName( const String& _name )
	{
		m_Impl->m_SkeletonName = _name;
	}

	const String& Mesh::GetSkeletonName() const
	{
		return m_Impl->m_SkeletonName;
	}

	bool Mesh::SetBonesWithDataOwnership(unsigned int* _boneIndBuffer, unsigned int _boneCount)
	{
		return m_Impl->SetBonesWithDataOwnership(_boneIndBuffer,_boneCount);
	}

	bool Mesh::SetVertexWeightBufferWithDataOwnership(float* _weightBuffer, unsigned int _weightCount)
	{
		return m_Impl->SetVertexWeightBufferWithDataOwnership(_weightBuffer,_weightCount);
	}

	bool Mesh::SetBoneInfluenceCountIndexBufferWithDataOwnership(unsigned int* _boneInfCountIndsBuffer, unsigned int _count)
	{
		return m_Impl->SetBoneInfluenceCountIndexBufferWithDataOwnership(_boneInfCountIndsBuffer, _count);
	}

	bool Mesh::SetBoneIndexBufferWithDataOwnership(unsigned int* _boneIndexBuffer, unsigned int _count)
	{
		return m_Impl->SetBoneIndexBufferWithDataOwnership(_boneIndexBuffer, _count);
	}

	bool Mesh::SetVertexWeightIndexBufferWithDataOwnership(unsigned int* _weightIndexBuffer, unsigned int _count)
	{
		return m_Impl->SetVertexWeightIndexBufferWithDataOwnership(_weightIndexBuffer,_count);
	}

	void Mesh::AddInverseBindMatrix(const Matrix44& _matrix)
	{
		m_Impl->AddInverseBindMatrix(_matrix);
	}

	void Mesh::SetBindShapeMatrix(const Matrix44& _matrix)
	{
		m_Impl->SetBindShapeMatrix(_matrix);
	}

	const Mesh::TInvBindMatrixArray& Mesh::GetInvBindMatricesArray() const
	{
		return m_Impl->GetInvBindMatricesArray();
	}

	const Matrix44& Mesh::GetBindShapeMatrix() const
	{
		return m_Impl->GetBindShapeMatrix();
	}

	void Mesh::SetPolygonMode(PolygonMode _polygonMode)
	{
		m_Impl->SetPolygonMode(_polygonMode);
	}
}