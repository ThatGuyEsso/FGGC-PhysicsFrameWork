#pragma once
#include "Collider.h"
#include "Vector3D.h"
#include "Collision.h"
class AABoxCollider : public Collider, public Collision {

public:
	AABoxCollider();
	AABoxCollider(Transform* transform, Vector3D size);
	void SetHalfSize(Vector3D size) { _halfSize = size; }
	void DynamicResize();
	Vector3D GetHalfSize() { return _halfSize; }
	bool AABBvsSphereCollision(SphereCollider* sphere, AABoxCollider* AABB);
	bool AABoxOnAABoxCollision(AABoxCollider* other);
	Vector3D GetMaxSize();
	Vector3D GetMinSize();
	void AABBReflection(Collider* other);
	Vector3D Support(Collider* other, Vector3D collisionAxis);
	Vector3D GenerateContacts(Collider* other, Vector3D collisionAxis);
	bool GJKIntersection(Collider* other, Vector3D initAxis);
	std::vector<Vector3D> CalculateMinkowskiDifference(Collider* other);
	void CaluclateVertices();
	Vector3D FurthestPoint(Vector3D dir);
	bool HandleSimplex(Vector3D dir);
private:

protected:
	Vector3D _halfSize;
	std::vector<Vector3D> vertices;
protected:
	bool CollisionCheck(Collider* other);
};