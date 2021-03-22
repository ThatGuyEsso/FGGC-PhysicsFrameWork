#include "Appearance.h"
#include "GameObject.h"

Appearance::Appearance(Geometry geometry, Material material, ID3D11ShaderResourceView* textureRV)
{


	_material = material;
	_geometry = geometry;
	_textureRV = textureRV;
	_type = ComponentType::Appearance;
}

Appearance::~Appearance()
{
	_textureRV = nullptr;

}

void Appearance::UpdateComponent(float deltaTime)
{
	//Needed to interface with components but has no implementation here
}






