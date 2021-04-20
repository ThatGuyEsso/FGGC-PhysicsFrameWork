#pragma once
#include "Collider.h"
#include "Collision.h"

class SphereCollider: public Collider, public Collision, public Component {
	 
public:
	SphereCollider();
	SphereCollider(Transform* transform, float radius);
	void SetRadius(float radius) { _radius = radius; }
	float GetRadius() { return _radius; }
	bool AABBvsSphereCollision(SphereCollider* sphere, AABoxCollider* AABB);
	void Reflection(CollisionData* contactData);
	Vector3D Support(Collider* other, Vector3D direction);

	bool GJKIntersection(Collider* other, Vector3D initAxis);
	void GenerateContacts(Collider* currtCollider, Collider* otherCollider, CollisionData* contactData);
	Vector3D FurthestPoint(Vector3D dir);
	
	//Compnonent Interface
	GameObject* GetOwner() override { return Component::_owner; }
	void SetOwner(GameObject* newOwner)override { Component::_owner = newOwner; }
	Component* GetClassType() { return this; }
	void UpdateComponent(float deltaTime);

private:
	float _radius;

private:
	bool CollisionCheck(Collider* other);
	bool SphereOnSphereCollision(SphereCollider* other);
};