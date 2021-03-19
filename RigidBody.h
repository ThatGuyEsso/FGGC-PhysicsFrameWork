#pragma once
#include "ParticleModel.h"



class RigidBody :public ParticleModel
{
public:
	RigidBody();
	RigidBody(Transform* transform, Vector3D InitialVelocity, Vector3D acceleration, bool useGravity);
	void CalulateTorgue(Vector3D force, Vector3D contactPoint);

	void CalculateAngularAcceleration();
	void CalculateAngularVelocity(float deltaTime);
	float CalculateDamping(float deltaTime);
	void SetInertiaTensorBox(float x, float y, float z);
	void Update(float t);
	void ApplyRotForce(Vector3D force, Vector3D point, float deltaTime);
	void StopObject();
protected:
private:
	XMFLOAT3X3 _intertiaTensor;
	float _angularDamping =0.5f;
	Vector3D _angularAcceleration;
	Vector3D _torgue;
	Vector3D _angularVelocity;
};

