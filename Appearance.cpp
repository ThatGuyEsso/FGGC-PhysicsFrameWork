#include "Appearance.h"

Appearance::Appearance(Geometry geometry, Material material, ID3D11ShaderResourceView* textureRV)
{


	_material = material;
	_geometry = geometry;
	_textureRV = textureRV;
}

Appearance::~Appearance()
{
	_textureRV = nullptr;

}

void Appearance::CalculateCentreOfMass(SimpleVertex vertices[])
{
	Vector3D sum = Vector3D();
	for (int i = 0; i < sizeof(vertices); i++) {

		Vector3D converstion = Vector3D(vertices[i].Pos.x, vertices[i].Pos.y, vertices[i].Pos.z);

		sum += converstion;
	}

	_centreOfMass = sum / sizeof(vertices);

}




