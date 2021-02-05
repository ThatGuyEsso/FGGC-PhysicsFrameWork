#pragma once

#include <d3d11_1.h>
#include "Appearance.h"


class Graphics {

private:
	Appearance* _appearance;

public:
	Graphics(Appearance* appearance);
	~Graphics();
	void Draw(ID3D11DeviceContext* pImmediateContext);
};