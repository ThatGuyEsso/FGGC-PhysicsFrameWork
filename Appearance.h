#pragma once
#include "Structs.h"
#include "Structures.h"
#include "Component.h"

class Appearance: public Component{
private:
	Geometry _geometry;
	Material _material;
	ID3D11ShaderResourceView* _textureRV;


public:

	Appearance(Geometry geometry, Material material, ID3D11ShaderResourceView* textureRV);
	~Appearance();
	//Compnonent Interface
	GameObject* GetOwner() override { return _owner; }
	void SetOwner(GameObject* newOwner)override { _owner = newOwner; };
	Component* GetClassType() { return this; }
	void UpdateComponent(float deltaTime);

	ID3D11ShaderResourceView* GetTextureRV() const { return _textureRV; }
	void SetTextureRV(ID3D11ShaderResourceView* textureRV) { _textureRV = textureRV; }

	//Appearance component getters
	Material GetMaterial() {return _material; }
	Geometry GetGeomentry()  { return _geometry; }
	bool HasTexture()  { return _textureRV ? true : false; }

	//Geometry Getters
	ID3D11Buffer** GetVertexBuffer() { return &_geometry.vertexBuffer; }
	UINT* GetVertexStride()  { return &_geometry.vertexBufferStride; }
	UINT* GetVertexBufferOffset() { return &_geometry.vertexBufferOffset; }

	
};