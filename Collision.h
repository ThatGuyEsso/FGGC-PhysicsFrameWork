#pragma once

class SphereCollider;
class AABoxCollider;
 

class Collision {

public:
	virtual bool AABBvsSphereCollision(SphereCollider* sphere, AABoxCollider* AABB) = 0;
	virtual void AABBReflection(Collider* other) = 0;
	virtual bool GJKIntersection(Collider* other, Vector3D initDir)= 0;
	virtual std::vector<Vector3D> CalculateMinkowskiDifference(Collider* other) = 0;
	virtual bool HandleSimplex(Vector3D dir) = 0;

protected:
	std::vector<Vector3D> _simplex;
	std::vector<Vector3D> _minKowSkiDifference;

};