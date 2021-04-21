#pragma once
#include "Structs.h"
#include "OBJLoader.h"
static const float defaultMass = 2.0f;
static const float DefaultDrag = 1.05f;
static const float Gravity = -98.1f;

static constexpr float FPS = 60.0f;

static  SimpleVertex CubeVertices[] =
{
	{ XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT2(1.0f, 0.0f) },
	{ XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT2(0.0f, 0.0f) },
	{ XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, 1.0f) },
	{ XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT2(1.0f, 1.0f) },

	{ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT2(0.0f, 0.0f) },
	{ XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT2(1.0f, 0.0f) },
	{ XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT2(1.0f, 1.0f) },
	{ XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT2(0.0f, 1.0f) },

	{ XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT2(0.0f, 1.0f) },
	{ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT2(1.0f, 1.0f) },
	{ XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT2(1.0f, 0.0f) },
	{ XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, 0.0f) },

	{ XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT2(1.0f, 1.0f) },
	{ XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT2(0.0f, 1.0f) },
	{ XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT2(0.0f, 0.0f) },
	{ XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT2(1.0f, 0.0f) },

	{ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT2(0.0f, 1.0f) },
	{ XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT2(1.0f, 1.0f) },
	{ XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT2(1.0f, 0.0f) },
	{ XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT2(0.0f, 0.0f) },

	{ XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT2(1.0f, 1.0f) },
	{ XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT2(0.0f, 1.0f) },
	{ XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, 0.0f) },
	{ XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT2(1.0f, 0.0f) },
};
static  SimpleVertex PlaneVertices[] =
{
	{ XMFLOAT3(-1.0f, -1.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT2(0.0f, 5.0f) },
	{ XMFLOAT3(1.0f, -1.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT2(5.0f, 5.0f) },
	{ XMFLOAT3(1.0f, 1.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT2(5.0f, 0.0f) },
	{ XMFLOAT3(-1.0f, 1.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT2(0.0f, 0.0f) },
};

static   WORD CubeIndices[] =
{
	3, 1, 0,
	2, 1, 3,

	6, 4, 5,
	7, 4, 6,

	11, 9, 8,
	10, 9, 11,

	14, 12, 13,
	15, 12, 14,

	19, 17, 16,
	18, 17, 19,

	22, 20, 21,
	23, 20, 22
};
static  WORD PlaneIndices[] =
{
	0, 3, 1,
	3, 2, 1,
};

static Material shinyMaterial = Material(XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f),
	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f), 10.0f);



static Material noSpecMaterial= Material(XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f),
	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f), 0.0f);



static Material depth1Mat = Material(XMFLOAT4(1.0f, 0.3f, 0.3f, 1.0f),
	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f), 10.0f);
static Material depth2Mat = Material(XMFLOAT4(0.3f, 0.3f, 1.0f, 1.0f),
	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f), 10.0f);
static Material depth3Mat = Material(XMFLOAT4(0.3f, 1.0f, 0.3f, 1.0f),
	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f), 10.0f);


static Geometry GetDonutMesh(ID3D11Device* _pd3dDevice) {
	Geometry donut;
	MeshData objMeshData = OBJLoader::Load("donut.obj", _pd3dDevice);
	donut.indexBuffer = objMeshData.IndexBuffer;
	donut.numberOfIndices = objMeshData.IndexCount;
	donut.vertexBuffer = objMeshData.VertexBuffer;
	donut.vertexBufferOffset = objMeshData.VBOffset;
	donut.vertexBufferStride = objMeshData.VBStride;
	return donut;
}


static Geometry GetSphereMesh(ID3D11Device* _pd3dDevice) {
	Geometry sphere;
	MeshData objMeshData = OBJLoader::Load("sphere.obj", _pd3dDevice);
	sphere.indexBuffer = objMeshData.IndexBuffer;
	sphere.numberOfIndices = objMeshData.IndexCount;
	sphere.vertexBuffer = objMeshData.VertexBuffer;
	sphere.vertexBufferOffset = objMeshData.VBOffset;
	sphere.vertexBufferStride = objMeshData.VBStride;
	return sphere;
}


static Geometry GetCubeMesh(ID3D11Buffer* iBuffer, ID3D11Buffer* vBuffer) {

	Geometry cubeGeometry;
	cubeGeometry.indexBuffer = iBuffer;
	cubeGeometry.vertexBuffer = vBuffer;
	cubeGeometry.numberOfIndices = 36;
	cubeGeometry.vertexBufferOffset = 0;
	cubeGeometry.vertexBufferStride = sizeof(SimpleVertex);
	cubeGeometry.vertices = &CubeVertices[0];
	cubeGeometry.numberOfVertices = 4;
	return cubeGeometry;
}

static Geometry GetPlaneMesh(ID3D11Buffer* iBuffer, ID3D11Buffer* vBuffer) {
	Geometry planeGeometry;
	planeGeometry.indexBuffer = iBuffer;
	planeGeometry.vertexBuffer = vBuffer;
	planeGeometry.numberOfIndices = 6;
	planeGeometry.vertexBufferOffset = 0;
	planeGeometry.vertexBufferStride = sizeof(SimpleVertex);
	planeGeometry.vertices = &PlaneVertices[0];
	planeGeometry.numberOfVertices = 4;
	return planeGeometry;
}