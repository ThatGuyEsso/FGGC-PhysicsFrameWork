#pragma once

#include "ParticleModel.h"
class ContactResolver
{
public:
	ContactResolver(ParticleModel* initBody, ParticleModel* otherBody, float resitution, Vector3D contactNormal,float pentration);
	ContactResolver(ParticleModel* initBody, ParticleModel* otherBody, float resitution, Vector3D contactNormal, Vector3D contactPoint, float pentration);
	ParticleModel* _bodies[2];
	float _restitution;
	Vector3D _contactNormal;
	Vector3D _contactPoint;
	float _penetration;
	void  Resolve(float duration);
protected:

	float CalculateSeparatingVelocity() const;

	bool isRigidBody;
private:
	void ResolveVelocity(float duration);
	void ResolveInterPentration(float duration);
};
