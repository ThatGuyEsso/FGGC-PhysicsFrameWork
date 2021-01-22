#include "ParticleModel.h"


ParticleModel::ParticleModel(Transform* transform)
{
	_transform = transform;
}

ParticleModel::~ParticleModel()
{
	_transform = nullptr;
}

void ParticleModel::MoveRight()
{
	//get position
	Vector3D currPos = _transform->GetPosition();
	//update Position
	_transform->SetPosition(Vector3D(currPos.show_X() + _moveAmount,currPos.show_Y(),currPos.show_Z()));
}

void ParticleModel::MoveLeft()
{
	//get position
	Vector3D currPos = _transform->GetPosition();
	//update Position
	_transform->SetPosition(Vector3D(currPos.show_X() - _moveAmount, currPos.show_Y(), currPos.show_Z()));
}

void ParticleModel::MoveForward()
{
	//get position
	Vector3D currPos = _transform->GetPosition();
	//update Position
	_transform->SetPosition(Vector3D(currPos.show_X(), currPos.show_Y(), currPos.show_Z()+_moveAmount));
}

void ParticleModel::MoveBackwards()
{
	//get position
	Vector3D currPos = _transform->GetPosition();
	//update Position
	_transform->SetPosition(Vector3D(currPos.show_X(), currPos.show_Y(), currPos.show_Z() - _moveAmount));
}
