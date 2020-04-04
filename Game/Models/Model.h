#ifndef __Model_h__
#define __Model_h__

namespace Game
{
	class Model
	{
	public:
		Model(){}
		virtual ~Model(){}

		virtual float* GetVertices()					{	return NULL;	}
		virtual unsigned int* GetIndices()				{	return NULL;	}
		virtual unsigned int TriangleCount()			{	return 0;		}
		virtual unsigned int GetVerticesCount()			{	return 0;		}
	};
}

#endif