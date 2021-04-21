#include "RigidBody.h"
#include "GameObject.h"
RigidBody::RigidBody() :ParticleModel::ParticleModel()
{
	_area = 2.0f;
	_dragCoEfficient = 10.5f;
	SetInertiaTensorBox(1.0f, 1.0, 1.0f);
	_angularVelocity = Vector3D();
	Component::_type = ComponentType::PhysicModel;
}

RigidBody::RigidBody(Transform* transform, Vector3D InitialVelocity, Vector3D acceleration, bool useGravity)
	:ParticleModel(transform,InitialVelocity,acceleration,useGravity)
{	
	_area = 2.0f;
	_dragCoEfficient = 10.5f;
	SetInertiaTensorBox(1.0f, 1.0, 1.0f);
	_angularVelocity = Vector3D();
	Component::_type = ComponentType::PhysicModel;
}



void RigidBody::CalculateTorgue(Vector3D force, Vector3D contactPoint)
{
	Vector3D centre = GetOwner()->GetCentreOfMass();
	Vector3D newTorgue = (centre + contactPoint).cross_product(force);
	
	_torgue = newTorgue;
	
}

void RigidBody::ApplyImpulse(Vector3D point, Vector3D force)
{
	if (_bodyMode != BodyMode::Static) {
		_currentVelocity *= _deltaTime;

	/*	Vector3D resultantToPoint = _transform->GetPosition() + point -force;


		ApplyRotForce(resultantToPoint, point, _deltaTime);*/
	}

}

void RigidBody::ApplySeparation(Vector3D Separation)
{
	if (_bodyMode != BodyMode::Static) {
		_transform->SetPosition(Separation);
	}
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

void RigidBody::SetInertiaTensorSphere(float radius)
{
	float tensorVal = ((2.0f / 5.0f) * _mass * radius * radius);
	//First row
	_intertiaTensor._11 = tensorVal;
	_intertiaTensor._12 = 0;
	_intertiaTensor._13 = 0;
	//Second Row
	_intertiaTensor._21 = 0;
	_intertiaTensor._22 = tensorVal;
	_intertiaTensor._23 = 0;
	//THird row
	_intertiaTensor._31 = 0;
	_intertiaTensor._32 = 0;
	_intertiaTensor._33 = tensorVal;
}

void RigidBody::UpdateComponent(float deltaTime)
{
	Update(deltaTime);
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
	switch (_bodyMode)
	{
	case RigidBody::BodyMode::Static:
		break;
	case RigidBody::BodyMode::Dynamic:
	
		CalculateAngularAcceleration();
		CalculateAngularVelocity(t);
		ParticleModel::Update(t);
		_torgue = Vector3D();
		break;

	}


}

void RigidBody::ApplyRotForce(Vector3D force, Vector3D point, float deltaTime)
{
	CalculateTorgue(force, point);
}

void RigidBody::StopObject()
{
	ParticleModel::StopObject();
	_torgue = Vector3D();
	_angularVelocity = Vector3D();
	_angularAcceleration = Vector3D();
}

Vector3D RigidBody::DragTurbFlow(Vector3D velocity, float dragFactor)
{
	float speed = velocity.magnitude();
	Vector3D velNorm = velocity.normalization();
	float dragMag = 0.5f *_area* dragFactor * _density * speed * speed;

	Vector3D drag = velNorm * -dragMag;


	return drag;
}



