#include "SphereCollider.h"
#include "GameObject.h"
#include "AABoxCollider.h"
#include <algorithm>
SphereCollider::SphereCollider():Collider::Collider(), Collision::Collision() {

	_radius = 1.0f;
	_colliderType = ColliderType::Sphere;
}

SphereCollider::SphereCollider(Transform* transform, float radius): Collider::Collider(transform), Collision::Collision() {

	_radius = radius;
	_colliderType = ColliderType::Sphere;
}

bool SphereCollider::AABBvsSphereCollision(SphereCollider* sphere, AABoxCollider* AABB)
{
	float x = std::max<float>(AABB->GetMinSize().x, (std::min<float>(sphere->GetTransform()->GetPosition().x, AABB->GetMaxSize().x)));
	float y = std::max<float>(AABB->GetMinSize().y, (std::min<float>(sphere->GetTransform()->GetPosition().y, AABB->GetMaxSize().y)));
	float z = std::max<float>(AABB->GetMinSize().z, (std::min<float>(sphere->GetTransform()->GetPosition().z, AABB->GetMaxSize().z)));

	float distance = std::sqrt((x - sphere->GetTransform()->GetPosition().x) *
		(x - sphere->GetTransform()->GetPosition().x) +
		(y - sphere->GetTransform()->GetPosition().y) *
		(y - sphere->GetTransform()->GetPosition().y) +
		(z - sphere->GetTransform()->GetPosition().z) *
		(z - sphere->GetTransform()->GetPosition().z));

	return distance < sphere->GetRadius();
}

void SphereCollider::AABBReflection(Collider* other)
{
	RigidBody* rb = GetOwner()->GetComponent<RigidBody>();
	if (rb) {

		//Calculate Normal to angle of insidence
		Vector3D vel  = rb->GetCurrentVelocity();
		//Currently Use objects position but in future use actual contact point

		Vector3D point = _transform->GetPosition();

		Vector3D normalToPoint = vel.cross_product(point);


		Vector3D reflection = vel -  normalToPoint.normalization()* (vel.dot_product(point))*2;

	
		rb->SetCurrentVelocity(reflection);
		rb->SetCurrentAcceleration(reflection.normalization()* rb->GetCurrentAcceleration().magnitude());


	}


	
}

Vector3D SphereCollider::Support(Collider* other, Vector3D direction)
{
	Vector3D supportPoint = FurthestPoint(direction) - other->FurthestPoint(direction*-1);
	return supportPoint;
}

Vector3D SphereCollider::GenerateContacts(Collider* other, Vector3D collisionAxis)
{

	return Vector3D();
}

bool SphereCollider::GJKIntersection(Collider* other, Vector3D initAxis)
{
	//Create new Simplex
	_simplex.clear();

	//get first arbitary point
	Vector3D initSimplex = Support(other, initAxis).normalization();
	//add startubg point to simplex
	_simplex.push_back(initSimplex);

	Vector3D dirToOrigin = (Vector3D() - _simplex[0]).normalization();

	while (true){
		//Second simplex vertex
		Vector3D A = Support(other, dirToOrigin);

		//checks if new line segmenet passes origin
		if (A.dot_product(dirToOrigin) < 0) {
			return false;//Shapes Did not intersect;
		}

		_simplex.push_back(A);
	}


	return false;
}

std::vector<Vector3D> SphereCollider::CalculateMinkowskiDifference(Collider* other)
{
	return std::vector<Vector3D>();
}

Vector3D SphereCollider::FurthestPoint(Vector3D dir)
{
	//calculate the farthest point from the orign in the direction of the other collider in collision
	Vector3D furtuestPoint = _transform->GetPosition() + dir * _radius;
	return furtuestPoint;
}

bool SphereCollider::HandleSimplex(Vector3D dir)
{
	return false;
}

bool SphereCollider::CollisionCheck(Collider* other)
{

		switch (other->GetColliderType()) {
			case ColliderType::Sphere:
			return SphereOnSphereCollision((SphereCollider*)other);
	
			case ColliderType::AABB:
				Vector3D dirToOrigin = (other->GetTransform()->GetPosition() - _transform->GetPosition()).normalization();

				bool isIntersecting = GJKIntersection(other, dirToOrigin);
		

			default:
				return false;
		}
	

	



}

bool SphereCollider::SphereOnSphereCollision(SphereCollider* other)
{
	float distance = other->GetTransform()->GetPosition().distance(_transform->_position);
	float sum = other->GetRadius() + _radius;
	return distance <= sum;
	
}


