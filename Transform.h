#pragma once
#include "Vector3D.h"


class Transform {

public:
	//Physical properties
	Vector3D _position;
	Vector3D _rotation;
	Vector3D _scale;

public:

	Transform();
	Transform(Vector3D position, Vector3D rotation, Vector3D scale);

	// Setters and Getters for position/rotation/scale
	void SetPosition(Vector3D position) { _position = position; }

	Vector3D GetPosition() const { return _position; }

	void SetScale(Vector3D scale) { _scale = scale; }


	Vector3D GetScale() const { return _scale; }

	void SetRotation(Vector3D rotation) { _rotation = rotation; }


	Vector3D GetRotation() const { return _rotation; }
};