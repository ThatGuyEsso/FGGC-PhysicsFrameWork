#include "Appearance.h"
#include "GameObject.h"

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






