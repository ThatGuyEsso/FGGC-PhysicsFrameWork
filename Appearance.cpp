#include "Appearance.h"
#include "GameObject.h"

Appearance::Appearance(Geometry geometry, Material material, ID3D11ShaderResourceView* textureRV)
{

	_type = ComponentType::Appearance;
	_material = material;
	_geometry = geometry;
	_textureRV = textureRV;

}

Appearance::Appearance()
{

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

void Appearance::DrawGUI()
{
	ImGui::SetNextWindowSize(ImVec2(500.0f, 10.0f));
	if (ImGui::Begin("Apperance")) {

	}
	ImGui::End();
}






