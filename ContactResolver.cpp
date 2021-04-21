#include "ContactResolver.h"

ContactResolver::ContactResolver(ParticleModel* initBody, ParticleModel* otherBody, float resitution, Vector3D contactNormal, float pentration)

{
	 isRigidBody = false;
	 _bodies[0] = initBody;
	 _bodies[1] = otherBody;
	 _restitution = resitution;
	 _contactNormal = contactNormal;
	 _penetration = pentration;
	
}

ContactResolver::ContactResolver(ParticleModel* initBody, ParticleModel* otherBody, float resitution, Vector3D contactNormal, Vector3D contactPoint, float pentration)
{
	isRigidBody = true;
	_bodies[0] = initBody;
	_bodies[1] = otherBody;
	_restitution = resitution;
	_penetration = pentration;
	_contactNormal = contactNormal;
	_contactPoint = contactPoint;
}

void ContactResolver::Resolve(float duration)
{
	ResolveVelocity(duration);
	ResolveInterPentration( duration);
}

float ContactResolver::CalculateSeparatingVelocity() const

{
	Vector3D relativeVel = _bodies[0]->GetCurrentVelocity();
	if (_bodies[1]) relativeVel = relativeVel- _bodies[1]->GetCurrentVelocity();
	return relativeVel.dot_product( _contactNormal);

}

void ContactResolver::ResolveVelocity(float duration)
{
	if (_penetration <= 0) return;



	/*float separatingVel = CalculateSeparatingVelocity();

	if (separatingVel > 0) {
		return;
	}

	float newSepVel = -separatingVel * _restitution;

	float changeInVel = newSepVel - separatingVel;


	float massSum = _bodies[0]->GetMass(); 
	if (_bodies[1])massSum += _bodies[1]->GetMass();

	float impulseMag = changeInVel / massSum;

	Vector3D impulsePerunitMass = _contactNormal * impulseMag;
	if (isRigidBody) {
		_bodies[0]->ApplyImpulse(_bodies[0]->GetCurrentVelocity() + impulsePerunitMass * _bodies[0]->GetMass(),_contactPoint);

		
		if (_bodies[1]) {
			_bodies[1]->ApplyImpulse(_bodies[1]->GetCurrentVelocity() + impulsePerunitMass * -_bodies[1]->GetMass(), _contactPoint);
		}
	}
	else {
		_bodies[0]->ApplyImpulse(_bodies[0]->GetCurrentVelocity() + impulsePerunitMass * _bodies[0]->GetMass());


		if (_bodies[1]) {
			_bodies[1]->ApplyImpulse(_bodies[1]->GetCurrentVelocity() + impulsePerunitMass * -_bodies[1]->GetMass());
		}
	}*/
	
}

void ContactResolver::ResolveInterPentration(float duration)
{
	if (_penetration <= 0) return;
	float massSum = _bodies[0]->GetMass();
	if (_bodies[1])massSum += _bodies[1]->GetMass();

	Vector3D movePerUnitMass = _contactNormal * (_penetration / massSum);

	Vector3D bodyMovement[2];
	bodyMovement[0] = movePerUnitMass * _bodies[0]->GetMass();

	if (_bodies[1]) {
		bodyMovement[1] = movePerUnitMass * -_bodies[1]->GetMass();
	}
	
	_bodies[0]->ApplySeparation((_bodies[0]->GetTransform()->GetPosition() + bodyMovement[0]));
	if (_bodies[1]) 
		_bodies[1]->ApplySeparation((_bodies[1]->GetTransform()->GetPosition() + bodyMovement[1]));
}
