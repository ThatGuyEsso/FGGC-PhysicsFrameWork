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

	float _dragCoEfficient;
	float _density;
	bool _useGravity;
	bool _useTurbularFlow;
	Vector3D _netForce;
	Vector3D _acceleration;
	Transform* _transform;
	Vector3D _currentVelocity;
	//temporary until collison is implemented
	
	ParticleEquation _equationUsed;

	std::vector<Vector3D> _forces; //all forces acting on the objects

	
protected:
	void UpdateNetForce();
	void UpdateAccel();

public:
	float _deltaTime;
	ParticleModel();
	ParticleModel(Transform* transform, Vector3D InitialVelocity, Vector3D acceleration, bool useGravity);
	ParticleModel(Transform* transform, Vector3D InitialVelocity, Vector3D acceleration, float mass, bool useGravity);
	ParticleModel(Transform* transform, Vector3D InitialVelocity, Vector3D acceleration, ParticleEquation equationType, bool useGravity);
	ParticleModel(Transform* transform, Vector3D InitialVelocity, Vector3D acceleration, ParticleEquation equationType, float mass, bool useGravity);
	~ParticleModel();

	virtual void Update(float t);
	virtual void StopObject();
	virtual void ApplyImpulse(Vector3D point, Vector3D force);
	virtual void ApplyImpulse(Vector3D force);
	virtual void ApplySeparation(Vector3D Separation);
	Vector3D ResolveImpulse(float thisMass, float otherMass, Vector3D thisVel, Vector3D otherVel, float restitution);
	void DrawGUI();

	//Compnonent Interface
	GameObject* GetOwner() override { return _owner; }
	void SetOwner(GameObject* newOwner)override { _owner = newOwner; }
	Component* GetClassType() { return this; }
	void UpdateComponent(float deltaTime);







	//particle movement functions
	void MoveWithConstVelocity(float deltaTime);
	void MoveWithConstAcceleration(float deltaTime);
	void StopParticle() { _currentVelocity = Vector3D(), _acceleration = Vector3D(); }
	void ApplyForce(Vector3D newForce);//adds a new force to particle
	void Move(float deltaTime);
	void ComputeMotion(float deltaTime);
	void ComputeMotionInFluid(float deltaTime);
	void AddGravity(float deltaTime);
	virtual Vector3D DragForce(Vector3D velocity, float dragFactor);
	virtual Vector3D DragLamFlow(Vector3D velocity, float dragFactor);
	virtual Vector3D DragTurbFlow(Vector3D velocity, float dragFactor);

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
	Transform* GetTransform() { return _transform; }

};