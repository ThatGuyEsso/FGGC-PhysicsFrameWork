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
private:

private:
	Vector3D _halfSize;
private:
	bool CollisionCheck(Collider* other);
};