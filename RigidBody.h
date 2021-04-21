#pragma once
#include "ParticleModel.h"



class RigidBody :public ParticleModel, public Component
{
public:
	enum class BodyMode {
		Static,
		Dynamic
	};
public:
	RigidBody();
	RigidBody(Transform* transform, Vector3D InitialVelocity, Vector3D acceleration, bool useGravity);

	void CalculateTorgue(Vector3D force, Vector3D contactPoint);
	void ApplyImpulse(Vector3D point, Vector3D force);
	void ApplySeparation(Vector3D Separation);
	//Compnonent Interface
	GameObject* GetOwner() override { return Component::_owner; }
	void SetOwner(GameObject* newOwner)override { Component::_owner = newOwner; }
	Component* GetClassType() { return this; }
	void UpdateComponent(float deltaTime);

	void CalculateAngularAcceleration();
	void CalculateAngularVelocity(float deltaTime);
	float CalculateDamping(float deltaTime);
	void SetInertiaTensorBox(float x, float y, float z);
	void SetInertiaTensorSphere(float radius);
	void Update(float t);
	void ApplyRotForce(Vector3D force, Vector3D point, float deltaTime);
	void StopObject();
	void SetRigidBodyMode(BodyMode newMode) { _bodyMode = newMode; }
	BodyMode GetBodyMode() { return _bodyMode; }
	Vector3D DragTurbFlow(Vector3D velocity, float dragFactor);
protected:
	XMFLOAT3X3 _intertiaTensor;
	float _angularDamping = 0.5f;
	float _area;
	Vector3D _angularAcceleration;
	Vector3D _torgue;
	Vector3D _angularVelocity;
	BodyMode _bodyMode = BodyMode::Dynamic;
};

