#pragma once
#include <cstring>
#include <d3d11.h>
#include <directxmath.h>

using namespace DirectX;

// This could be a class however its functionality is going to tie heavily into the HMD
//struct Camera
//{
//	XMFLOAT4	eye;
//	XMFLOAT4	at;	
//	XMFLOAT4	up;
//	
//	XMFLOAT4X4				view;
//	XMFLOAT4X4              projection;
//};
//
//struct RenderImage
//{
//	ID3D11RenderTargetView* _pRenderTargetView;
//	ID3D11Texture2D* _RenderTargetTexture;
//	ID3D11ShaderResourceView* _shaderResourceView;
//	ID3D11Buffer* _RenderImageVertexBuffer;
//	ID3D11Buffer* _RenderImageIndexBuffer;
//};


//struct ConstantBuffer
//{
//	XMMATRIX mWorld;
//	XMMATRIX mView;
//	XMMATRIX mProjection;
//	XMFLOAT4 DiffuseMtrl;
//	XMFLOAT4 DiffuseLight;
//	XMFLOAT4 AmbientMtrl;
//	XMFLOAT4 AmbientLight;
//	XMFLOAT4 SpecularMtrl;
//	XMFLOAT4 SpecularLight;
//	XMFLOAT3 LightVecW;
//	float gTime;
//	XMFLOAT3 EyePosW;
//	float SpecularPower;
//	unsigned int activeViewport;
//};

