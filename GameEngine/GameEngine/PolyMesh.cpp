#include "PolyMesh.h"

/*PolyMesh::PolyMesh(vector<Mesh*> _meshes)
{
	boundingBox = nullptr;
	setMeshes(_meshes);
}

PolyMesh::~PolyMesh()
{
	if (boundingBox)
		delete boundingBox;
}

int 
PolyMesh::getUniqueVertices(vector<Vec3> *verts)
{
	if (meshes.size() >= 1)
	{
		meshes[0]->getUniqueVertices(verts);
		for (int i = 1; i < meshes.size(); i++)
		{
			bool isVertInExisting = false;
			vector<Vec3> temp;
			meshes[i]->getUniqueVertices(&temp);

			for (int j = 0; j < temp.size(); j++)
			{
				for (int k = 0; k < verts->size(); k++)
				{
					if ((*verts)[k] == (temp)[j])
					{
						isVertInExisting = true;
						break;
					}
				}

				if (!isVertInExisting)
					verts->push_back((temp)[j]);
			}
		}

		return verts->size();
	}
}

void
PolyMesh::setMeshes(vector<Mesh*> _meshes)
{
	meshes = _meshes;

	if (boundingBox)
		delete boundingBox;

	boundingBox = new BoundingBox(*this);
}
*/