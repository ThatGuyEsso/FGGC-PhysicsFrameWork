#pragma once

class SphereCollider;
class AABoxCollider;
 

class Collision {

public:
	virtual bool AABBvsSphereCollision(SphereCollider* sphere, AABoxCollider* AABB) = 0;
	virtual void AABBReflection(Collider* other) = 0;
	
};