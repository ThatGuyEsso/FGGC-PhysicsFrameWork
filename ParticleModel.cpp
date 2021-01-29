#include "ParticleModel.h"
#include "DebugHelp.h"


ParticleModel::ParticleModel(Transform* transform, Vector3D InitialVelocity, Vector3D acceleration)
{
	_transform = transform;
	_currentVelocity = InitialVelocity;
	_acceleration = acceleration;
	_equationUsed = ParticleEquation::ConstantVelocity;
}

ParticleModel::ParticleModel(Transform* transform, Vector3D InitialVelocity, Vector3D acceleration, ParticleEquation equationType)
{
	_transform = transform;
	_currentVelocity = InitialVelocity;
	_acceleration = acceleration;
	_equationUsed = equationType;
}

ParticleModel::~ParticleModel()
{
	_transform = nullptr;
}

void ParticleModel::Update(float t) {

	switch (_equationUsed)
	{
	case ParticleModel::ParticleEquation::ConstantVelocity:
		MoveWithConstVelocity(t);
		break;
	case ParticleModel::ParticleEquation::ConstantAcceleration:
		MoveWithConstAcceleration(t);
		break;
	default:
		break;
	}

}

void ParticleModel::MoveWithConstVelocity(float deltaTime) {

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
	DebugHelp().OutPutValue("speed =", _currentVelocity.magnitude());
	 _currentVelocity = newVelocity;
	 DebugHelp().OutPutValue("speed =", _currentVelocity.magnitude());
}




void ParticleModel::MoveRight()
{
	SetCurrentVelocity(Vector3D(500.0f, 0.0f, 0.0f));
}

void ParticleModel::MoveLeft()
{
	SetCurrentVelocity(Vector3D(-20.0f, 0.0f, 0.0f));
}

void ParticleModel::MoveForward()
{
	_currentVelocity = Vector3D(0.0f, 0.0f, 0.01f);
}
void ParticleModel::MoveBackwards()
{
	SetCurrentVelocity(Vector3D(0.0f, 0.0f,-20.0f));
}


