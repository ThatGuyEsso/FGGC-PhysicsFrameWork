#include "RigidBody.h"
#include "GameObject.h"
RigidBody::RigidBody() :ParticleModel::ParticleModel()
{
	SetInertiaTensorBox(1.0f, 1.0, 1.0f);
	_angularVelocity = Vector3D();
}

RigidBody::RigidBody(Transform* transform, Vector3D InitialVelocity, Vector3D acceleration, bool useGravity)
	:ParticleModel(transform,InitialVelocity,acceleration,useGravity)
{
	SetInertiaTensorBox(1.0f, 1.0, 1.0f);
	_angularVelocity = Vector3D();
}

void RigidBody::CalulateTorgue(Vector3D force, Vector3D contactPoint)
{
	_torgue = (GetOwner()->GetCentreOfMass() +contactPoint).cross_product(force);

	
}

void RigidBody::SetInertiaTensorBox(float x, float y, float z)
{
	//First row
	_intertiaTensor._11 = (1.0f/12.0f*_mass)*((y*y)+(z*z));
	_intertiaTensor._12 = 0;
	_intertiaTensor._13 = 0;
	//Second Row
	_intertiaTensor._21 = 0;
	_intertiaTensor._22 = (1.0f / 12.0f * _mass) * ((x * x) + (z * z));
	_intertiaTensor._23 = 0;
	//THird row
	_intertiaTensor._31 = 0;
	_intertiaTensor._32 = 0;
	_intertiaTensor._33 = (1.0f / 12.0f * _mass) * ((x * x) + (y * y));



}

void RigidBody::CalculateAngularAcceleration()
{
	XMFLOAT3 accel;    
	XMVECTOR torgue = XMVectorSet(_torgue.x, _torgue.y, _torgue.z, 0.0f);
	XMMATRIX intertiaTensor = XMLoadFloat3x3(&_intertiaTensor);
	XMVECTOR inverseTensor;
	XMMatrixInverse(&inverseTensor, intertiaTensor);

	XMVECTOR result = torgue * inverseTensor;

	XMStoreFloat3(&accel, result);
	_angularAcceleration = Vector3D(accel.x, accel.y, accel.z);

}


void RigidBody::CalculateAngularVelocity(float deltaTime)
{
	_angularVelocity += _angularAcceleration * deltaTime;

	float damping= CalculateDamping(deltaTime);
	_angularVelocity *= damping;
	
	_transform->Rotate(_angularVelocity, deltaTime);
}

float RigidBody::CalculateDamping(float deltaTime)
{
	return pow(_angularDamping, deltaTime);

}

void RigidBody::Update(float t)
{
	ParticleModel::Update(t);


	CalculateAngularAcceleration();
	CalculateAngularVelocity(t);
	_torgue = Vector3D();
	
}

void RigidBody::ApplyRotForce(Vector3D force, Vector3D point, float deltaTime)
{
	CalulateTorgue(force, point);

	


}



