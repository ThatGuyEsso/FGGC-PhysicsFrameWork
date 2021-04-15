#pragma once
#include <vector>
#include "Contact.h"
class SphereCollider;
class AABoxCollider;
class Collider;
class Vector3D;
class Collision {
public:
	struct CollisionData {
		Contact* contacts; // contact array to write to

		int totalContacts; //max contacts array can hold
	};

public:

	Collision();
	virtual bool AABBvsSphereCollision(SphereCollider* sphere, AABoxCollider* AABB) = 0;
	virtual void AABBReflection(Vector3D contact) = 0;
	virtual bool GJKIntersection(Collider* other, Vector3D initDir)= 0;

	virtual bool HandleSimplex(Vector3D dir, Collider* a, Collider* b);
	virtual void LineCase(Vector3D dir,Collider* a , Collider* b);
	virtual bool TriangleCase(Vector3D dir, Collider* a, Collider* b);
	virtual bool TetrahedronCase(Vector3D dir, std::vector<Vector3D> tetrahedron);
	virtual bool SameSide(Vector3D v1, Vector3D v2, Vector3D v3, Vector3D v4, Vector3D point);
	virtual void GenerateContacts(Collider* currtCollider, Collider* otherCollider, CollisionData* contactData)=0;
	
	
protected:
	std::vector<Vector3D> _simplex = std::vector<Vector3D>();
	std::vector<Vector3D> _minKowSkiDifference;

};