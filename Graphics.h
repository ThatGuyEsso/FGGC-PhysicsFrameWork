#pragma once

#include <d3d11_1.h>
#include "Appearance.h"

class Graphics: public Component {

private:

public:
	Graphics();
	//Compnonent Interface
	GameObject* GetOwner() override { return _owner; }
	void SetOwner(GameObject* newOwner)override { _owner = newOwner; }
	Component* GetClassType() { return this; }
	void UpdateComponent(float deltaTime);

	void Draw(ID3D11DeviceContext* pImmediateContext);
};