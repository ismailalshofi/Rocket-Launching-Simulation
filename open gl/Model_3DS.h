#ifndef MODEL_3DS_H
#define MODEL_3DS_H
#include "GLTexture.h"
#include <stdio.h>
class Model_3DS
{
public:
	struct mVector
	{
		float x;
		float y;
		float z;
	};
	struct Vertex
	{
		float x;
		float y;
		float z;
	};
	struct Color
	{
		float r;
		float g;
		float b;
		float a;
	};
	struct Material
	{
		char name[80];
		GLTexture tex;
		bool textured;
		Color ambient;
		Color diffuse;
		Color specular;
	};
	struct ChunkHeader
	{
		unsigned short id;
		unsigned long  len;

	};
	struct MaterialFaces
	{
		unsigned short *subFaces;
		int numSubFaces;
		int MatIndex;
	};
	struct Object
	{
		char name[80];
		float *Vertexes;
		float *Normals;
		float *TexCoords;
		unsigned short *Faces;
		int numFaces;
		int numMatFaces;
		int numVerts;
		int numTexCoords;
		bool textured;
		MaterialFaces *MatFaces;
		mVector pos;
		mVector rot;
	};
	char *modelname;
	char *path;
	int numObjects;
	int numMaterials;
	int totalVerts;
	int totalFaces;
	bool shownormals;
	Material *Materials;
	Object *Objects;
	mVector pos;
	mVector rot;
	float scale;
	bool lit;
	bool visible;
	bool textured;
	void Load(char *name);
	void Draw();
	FILE *bin3ds;
	Model_3DS();
	virtual ~Model_3DS();
private:
	void IntColorChunkProcessor(long length, long findex, Color& c);
	void FloatColorChunkProcessor(long length, long findex, Color& c);
	void MainChunkProcessor(long length, long findex);
	void EditChunkProcessor(long length, long findex);
	void MaterialChunkProcessor(long length, long findex, int matindex);
	void MaterialNameChunkProcessor(long length, long findex, int matindex);
	void AmbientColorChunkProcessor(long length, long findex, int matindex);
	void DiffuseColorChunkProcessor(long length, long findex, int matindex);
	void SpecularColorChunkProcessor(long length, long findex, int matindex);
	void TextureMapChunkProcessor(long length, long findex, int matindex);
	void MapNameChunkProcessor(long length, long findex, int matindex);
	void ObjectChunkProcessor(long length, long findex, int objindex);
	void TriangularMeshChunkProcessor(long length, long findex, int objindex);
	void VertexListChunkProcessor(long length, long findex, int objindex);
	void TexCoordsChunkProcessor(long length, long findex, int objindex);
	void FacesDescriptionChunkProcessor(long length, long findex, int objindex);
	void FacesMaterialsListChunkProcessor(long length, long findex, int objindex, int subfacesindex);
	void CalculateNormals();
};
#endif MODEL_3DS_H
