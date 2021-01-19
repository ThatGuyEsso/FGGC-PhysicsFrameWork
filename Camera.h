#pragma once

#include <windows.h>
#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include "Vector3D.h"

using namespace DirectX;

class Camera
{
private:
	Vector3D _eye;
	Vector3D _at;
	Vector3D _up;

	FLOAT _windowWidth;
	FLOAT _windowHeight;
	FLOAT _nearDepth;
	FLOAT _farDepth;

	XMFLOAT4X4 _view;
	XMFLOAT4X4 _projection;

public:
	Camera(Vector3D position, Vector3D at, Vector3D up, FLOAT windowWidth, FLOAT windowHeight, FLOAT nearDepth, FLOAT farDepth);
	~Camera();

	void Update();

	XMFLOAT4X4 GetView() const { return _view; }
	XMFLOAT4X4 GetProjection() const { return _projection; }

	XMFLOAT4X4 GetViewProjection() const;

	Vector3D GetPosition() const { return _eye; }
	Vector3D GetLookAt() const { return _at; }
	Vector3D GetUp() const { return _up; }

	void SetPosition(Vector3D position) { _eye = position; }
	void SetLookAt(Vector3D lookAt) { _at = lookAt; }
	void SetUp(Vector3D up) { _up = up; }

	void Reshape(FLOAT windowWidth, FLOAT windowHeight, FLOAT nearDepth, FLOAT farDepth);
};

