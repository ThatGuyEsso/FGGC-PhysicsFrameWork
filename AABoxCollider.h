#pragma once
#include "Collider.h"
#include "Vector3D.h"
#include "Collision.h"
class AABoxCollider : public Collider, public Collision {

public:
	AABoxCollider();
	AABoxCollider(Transform* transform, Vector3D size);
	void SetHalfSize(Vector3D size);
	void DynamicResize();
	Vector3D GetHalfSize() { return _halfSize; }
	bool AABBvsSphereCollision(SphereCollider* sphere, AABoxCollider* AABB);
	bool AABoxOnAABoxCollision(AABoxCollider* other);
	Vector3D GetMaxSize();
	Vector3D GetMinSize();
	void Reflection(CollisionData* contactData);
	Vector3D Support(Collider* other, Vector3D collisionAxis);
	void GenerateContacts(Collider* currtCollider, Collider* otherCollider, CollisionData* contactData);
	void CalculateVertices();
	Vector3D FurthestPoint(Vector3D dir);
	Vector3D ClosesPointToPoint(Vector3D point);
	bool GJKIntersection(Collider* other, Vector3D initAxis);
	virtual Vector3D GetAxis(int axis);
	
private:

protected:
	Vector3D _halfSize;
	std::vector<Vector3D> _vertices;
	
protected:
	bool CollisionCheck(Collider* other);
};