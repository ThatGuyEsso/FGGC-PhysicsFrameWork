#pragma once
#include "Collider.h"
#include "Collision.h"

class SphereCollider: public Collider, public Collision {
	 
public:
	SphereCollider();
	SphereCollider(Transform* transform, float radius);
	void SetRadius(float radius) { _radius = radius; }
	float GetRadius() { return _radius; }
	bool AABBvsSphereCollision(SphereCollider* sphere, AABoxCollider* AABB);
	void AABBReflection(Collider* other);
	Vector3D Support(Collider* other, Vector3D direction);
	Vector3D GenerateContacts(Collider* other, Vector3D collisionAxis);
	bool GJKIntersection(Collider* other, Vector3D initAxis);

	Vector3D FurthestPoint(Vector3D dir);
	
private:
	float _radius;

private:
	bool CollisionCheck(Collider* other);
	bool SphereOnSphereCollision(SphereCollider* other);
};