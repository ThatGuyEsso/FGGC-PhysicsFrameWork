#pragma once
#include "Transform.h"
class ParticleModel {
public:
	enum class ParticleEquation {
		ConstantVelocity,
		ConstantAcceleration
	};

private:
	float _moveAmount = 0.2f;
	Vector3D _acceleration;
	Transform* _transform;
	Vector3D _currentVelocity;
	ParticleEquation _equationUsed;

public:
	

	ParticleModel(Transform* transform, Vector3D InitialVelocity, Vector3D acceleration);
	ParticleModel(Transform* transform, Vector3D InitialVelocity, Vector3D acceleration, ParticleEquation equationType);
	~ParticleModel();

	void Update(float t);
	void MoveRight();
	void MoveLeft();
	void MoveForward();
	void MoveBackwards();

	//particle movement functions
	void MoveWithConstVelocity(float deltaTime);
	void MoveWithConstAcceleration(float deltaTime);
	void StopParticle() { _currentVelocity = Vector3D(), _acceleration = Vector3D(); }


	//setters and getters for particle movement equations
	Vector3D GetCurrentVelocity() { return _currentVelocity; }
	void SetCurrentVelocity(Vector3D newVelocity);
	Vector3D GetCurrentAcceleration() { return _acceleration; }
	void SetCurrentAcceleration(Vector3D newAccel) { _acceleration = newAccel; }
	void SetEquationUsed(ParticleEquation newEquation) { _equationUsed = newEquation; }
	ParticleEquation GetEquationUsed() { return _equationUsed; }






};