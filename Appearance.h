#pragma once
#include "Structs.h"
class Appearance {
private:
	Geometry _geometry;
	Material _material;
	ID3D11ShaderResourceView* _textureRV;


public:

	Appearance(Geometry geometry, Material material, ID3D11ShaderResourceView* textureRV);
	~Appearance();



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