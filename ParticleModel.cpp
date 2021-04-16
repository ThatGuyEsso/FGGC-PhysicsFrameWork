#include "ParticleModel.h"
#include "DebugHelp.h"
#include "Commons.h"
#include "GameObject.h"
ParticleModel::ParticleModel()
{
	_useTurbularFlow = false;
	_collider = new SphereCollider(_transform, 0.3f);
	_type = ComponentType::PhysicModel;
}

ParticleModel::ParticleModel(Transform* transform, Vector3D InitialVelocity, Vector3D acceleration, bool useGravity)
{
	_transform = transform;
	_currentVelocity = InitialVelocity;
	_acceleration = acceleration;
	_equationUsed = ParticleEquation::ConstantVelocity;
	_mass = defaultMass;
	_useGravity = useGravity;
	_dragCoEfficient = DefaultDrag;
	_density = 1.225f;
	_useTurbularFlow = true;
	_collider = new SphereCollider(_transform, 0.3f);
	_type = ComponentType::PhysicModel;
}


ParticleModel::ParticleModel(Transform* transform, Vector3D InitialVelocity, Vector3D acceleration, ParticleEquation equationType, bool useGravity)
{

	_transform = transform;
	_currentVelocity = InitialVelocity;
	_acceleration = acceleration;
	_equationUsed = equationType;
	_mass = defaultMass;
	_useGravity = useGravity;
	_dragCoEfficient = DefaultDrag;
	_density = 1.0f;
	_useTurbularFlow = true;
	_collider = new SphereCollider(_transform, 1.0f);
	_type = ComponentType::PhysicModel;
}

ParticleModel::ParticleModel(Transform* transform, Vector3D InitialVelocity, Vector3D acceleration, float mass, bool useGravity)
{
	_transform = transform;
	_currentVelocity = InitialVelocity;
	_acceleration = acceleration;
	_equationUsed = ParticleEquation::ConstantVelocity;
	_mass = mass;
	_useGravity = useGravity;
	_dragCoEfficient = DefaultDrag;
	_density = 1.0f;
	_useTurbularFlow = false;
	_collider = new SphereCollider(_transform, 1.0f);
	_type = ComponentType::PhysicModel;
}

ParticleModel::ParticleModel(Transform* transform, Vector3D InitialVelocity, Vector3D acceleration, ParticleEquation equationType, float mass, bool useGravity)
{
	_transform = transform;
	_currentVelocity = InitialVelocity;
	_acceleration = acceleration;
	_equationUsed = equationType;
	_mass = mass;
	_useGravity = useGravity;
	_dragCoEfficient = DefaultDrag;
	_density = 1.0f;
	_useTurbularFlow = true;
	_collider = new SphereCollider(_transform, 1.0f);
	_type = ComponentType::PhysicModel;
}
ParticleModel::~ParticleModel()
{
	_transform = nullptr;
}
void ParticleModel::UpdateNetForce()
{
	//add all forces to the net force
	if (!_forces.empty()) {
		for (Vector3D force : _forces) {
			_netForce += force;
		}
	}
	else {
		_netForce = Vector3D();
	}



}
void ParticleModel::UpdateAccel() {
	_acceleration = _netForce / _mass;

	//DebugHelp().OutPutValue("acc X", _acceleration.x);
	//DebugHelp().OutPutValue("acc y", _acceleration.y);
	//DebugHelp().OutPutValue("acc z", _acceleration.z);

}

void ParticleModel::StopObject()
{
	_currentVelocity = Vector3D();
	_acceleration = Vector3D();
}


void ParticleModel::Update(float t) {

	//switch (_equationUsed)
	//{
	//case ParticleModel::ParticleEquation::ConstantVelocity:
	//	MoveWithConstVelocity(t);
	//	break;
	//case ParticleModel::ParticleEquation::ConstantAcceleration:
	//	MoveWithConstAcceleration(t);
	//	break;
	//default:
	//	break;
	//}

	//after each frame all forces applied to object are removed

	ComputeMotionInFluid(t);
	//ComputeMotion(t);
	

	if (_transform->GetPosition().y < 0) {
		_transform->SetPosition(Vector3D(_transform->GetPosition().x, 0, _transform->GetPosition().z));
		_currentVelocity = Vector3D(_currentVelocity.x, 0.0f, _currentVelocity.z);
		_acceleration = Vector3D(_acceleration.x, 0.0f, _acceleration.z);
	}
	//DebugHelp().OutPutValue("NetForce before frame", _netForce.magnitude());
	_forces.clear();
	UpdateNetForce();
}

void ParticleModel::SetOwner(GameObject* newOwner)
{
	_owner = newOwner;
	if (_collider != nullptr) {
		_collider->SetOwner(newOwner);
	}
}

void ParticleModel::UpdateComponent(float deltaTime)
{
	Update(deltaTime);
}


void ParticleModel::MoveWithConstVelocity(float deltaTime) {
	
	//if (_currentVelocity.magnitude() > 0) {
	//	DebugHelp().OutPutText("value is more than zero");
	//}
		_transform->SetPosition(_transform->GetPosition() += _currentVelocity* deltaTime);
	


}

void ParticleModel::MoveWithConstAcceleration(float deltaTime)
{
	// update world position of object by adding displacement to previously calculated position (look up suvat equations) 
	Vector3D newPosition = _currentVelocity * deltaTime + _acceleration * 0.5f * (deltaTime * deltaTime);
	_transform->SetPosition(_transform->GetPosition() += newPosition);

	_currentVelocity += _acceleration * deltaTime;
	//// update velocity of object by adding change relative to previously calculated velocity  
	// velocity = previous velocity + acceleration x deltaTime;
}

void ParticleModel::SetCurrentVelocity(Vector3D newVelocity)
{
	//DebugHelp().OutPutValue("speed =", _currentVelocity.magnitude());
	 _currentVelocity = newVelocity;
	 //DebugHelp().OutPutValue("speed =", _currentVelocity.magnitude());


}

void ParticleModel::SetCollider(Collider* coliider)
{
	delete _collider;
	_collider = coliider;
	if (_owner) _collider->SetOwner(_owner);
}

void ParticleModel::ApplyForce(Vector3D newForce)
{
	_forces.push_back(newForce);
	//if(newForce.magnitude()>0)
	//	DebugHelp().OutPutText("NewForce added");
}

void ParticleModel::Move(float deltaTime)
{
	
	if (_currentVelocity.magnitude() <= 0.008f) _currentVelocity = Vector3D();
	// update world position of object by adding displacement to previously calculated position (look up suvat equations) 
	Vector3D newPosition =_transform->GetPosition()+ _currentVelocity * deltaTime + _acceleration * 0.5f * (deltaTime * deltaTime);
	_transform->SetPosition(newPosition);
	
	//calculate new velocity
	_currentVelocity += _acceleration * deltaTime;

}

void ParticleModel::AddGravity()
{
	Vector3D gravity = Vector3D(0, 1, 0) * Gravity * _mass;

	ApplyForce(gravity);

}

void ParticleModel::ComputeMotion(float deltaTime)
{
	if (_useGravity) {
		AddGravity();

	

	}
	//Update netforces acting on gameobjects
	UpdateNetForce();

	//recalculate current acceleration
	UpdateAccel();

	Move(deltaTime);
}
void ParticleModel::ComputeMotionInFluid(float deltaTime)
{
	if (_useGravity) {
		
		AddGravity();

	/*	if (_transform->GetPosition().y <= _surfacePosition.y) {
			ApplyForce(Vector3D(0.0f, 1, 0) * Gravity * _mass * -1);
		}*/

	}

	//apply all drag forces
	ApplyForce(DragForce(_currentVelocity,_dragCoEfficient));

	//Update netforces acting on gameobjects
	UpdateNetForce();

	//recalculate current acceleration
	UpdateAccel();


	Move(deltaTime);
}
Vector3D ParticleModel::DragForce(Vector3D velocity, float dragFactor)
{
	if (_useTurbularFlow) {
		return DragTurbFlow(velocity, dragFactor);
	}
	else {
		return DragLamFlow(velocity,dragFactor);
	}
}


Vector3D ParticleModel::DragLamFlow(Vector3D velocity, float dragFactor)
{
	Vector3D drag = velocity * -dragFactor;
	return drag;
}
Vector3D ParticleModel::DragTurbFlow(Vector3D velocity, float dragFactor) {
	float speed = velocity.magnitude();
	Vector3D velNorm = velocity.normalization();
	float dragMag = 0.5f*dragFactor * _density* speed * speed;

	Vector3D drag = velNorm * -dragMag;

	return drag;

}





