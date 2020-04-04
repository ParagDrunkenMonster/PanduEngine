/********************************************************************
	filename: 	PANDUNavMesh
	author:		Parag Moni Boro
	
	purpose:	Game Engine created for learning
*********************************************************************/

#ifndef __PANDUNavMesh_h__
#define __PANDUNavMesh_h__

namespace Pandu
{
	class NavMesh;

	class NavPolygon
	{
	};

	//=====================================================================
	//Navigation Mesh
	class NavMesh
	{
	private:

		friend class NavPolygon;

		float * const m_Verts;
		unsigned int * const m_Indices;

	public:

		NavMesh(const float* const _verts, unsigned int vertsCount,const unsigned int* _indices,unsigned int indicesCount);
		~NavMesh();
	};
}

#endif