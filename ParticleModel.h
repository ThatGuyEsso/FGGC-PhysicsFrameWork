#pragma once
#include "Transform.h"
#include "Component.h"
#include <vector>
#include "SphereCollider.h"
#include "AABoxCollider.h"
class ParticleModel: public Component {
public:
	enum class ParticleEquation {
		ConstantVelocity,
		ConstantAcceleration
	};

protected:
	float _mass;
	float _drag;
	bool _useGravity;
	bool _useTurbularFlow;
	Vector3D _netForce;
	Vector3D _acceleration;
	Transform* _transform;
	Vector3D _currentVelocity;
	//temporary until collison is implemented
	Vector3D _surfacePosition;
	ParticleEquation _equationUsed;

	std::vector<Vector3D> _forces; //all forces acting on the objects

	Collider* _collider;
protected:
	void UpdateNetForce(float deltaTime);
	void UpdateAccel();
public:
	
	ParticleModel();
	ParticleModel(Transform* transform, Vector3D InitialVelocity, Vector3D acceleration, bool useGravity);
	ParticleModel(Transform* transform, Vector3D InitialVelocity, Vector3D acceleration, float mass, bool useGravity);
	ParticleModel(Transform* transform, Vector3D InitialVelocity, Vector3D acceleration, ParticleEquation equationType, bool useGravity);
	ParticleModel(Transform* transform, Vector3D InitialVelocity, Vector3D acceleration, ParticleEquation equationType, float mass, bool useGravity);
	~ParticleModel();

	virtual void Update(float t);
	virtual void StopObject();
	//component Interface
	void SetOwner(GameObject* newOwner);
	GameObject* GetOwner()override { return _owner; }

	Collider* GetCollider() { return _collider; }
	//particle movement functions
	void MoveWithConstVelocity(float deltaTime);
	void MoveWithConstAcceleration(float deltaTime);
	void StopParticle() { _currentVelocity = Vector3D(), _acceleration = Vector3D(); }
	void ApplyForce(Vector3D newForce) { _forces.push_back(newForce); }//adds a new force to particle
	void Move(float deltaTime);
	void ComputeMotion(float deltaTime);
	void ComputeMotionInFluid(float deltaTime);
	void AddGravity();
	Vector3D DragForce(Vector3D velocity, float dragFactor);
	Vector3D DragLamFlow(Vector3D velocity, float dragFactor);
	Vector3D DragTurbFlow(Vector3D velocity, float dragFactor);

	//setters and getters for particle movement equations
	Vector3D GetCurrentVelocity() { return _currentVelocity; }
	Vector3D GetCurrentAcceleration() { return _acceleration; }
	ParticleEquation GetEquationUsed() { return _equationUsed; }
	void SetCurrentVelocity(Vector3D newVelocity);
	void SetCurrentAcceleration(Vector3D newAccel) { _acceleration = newAccel; }
	void SetEquationUsed(ParticleEquation newEquation) { _equationUsed = newEquation; }

	//setters and getters for mass
	float GetMass() { return _mass; }
	void SetMass(float newMass) {  _mass= newMass; }
	void ToggleGravity(bool useGravity) { _useGravity = useGravity; }
	bool UsesGravity() { return _useGravity; }
	void SetSurfacePosition(Vector3D position) { _surfacePosition = position; }
	void SetCollider(Collider* coliider);

};