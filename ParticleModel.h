#pragma once
#include "Transform.h"
class ParticleModel {

private:
	float _moveAmount = 0.2f;
	Transform* _transform;

public:
	ParticleModel(Transform* transform);
	~ParticleModel();

	void MoveRight();
	void MoveLeft();
	void MoveForward();
	void MoveBackwards();
};