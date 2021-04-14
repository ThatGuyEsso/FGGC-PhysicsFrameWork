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

void SphereCollider::AABBReflection(Vector3D contact)
{
	RigidBody* rb = GetOwner()->GetComponent<RigidBody>();

	if (rb) {

		
		//Calculate Normal to angle of insidence
		Vector3D vel  = rb->GetCurrentVelocity();
		//Currently Use objects position but in future use actual contact point

	

		Vector3D normalToPoint = vel.cross_product(contact);


		Vector3D reflection = vel -  normalToPoint.normalization()* (vel.dot_product(contact))*2;

	
		rb->SetCurrentVelocity(reflection);
		rb->SetCurrentAcceleration(reflection.normalization()* rb->GetCurrentAcceleration().magnitude());


	}


	
}

Vector3D SphereCollider::Support(Collider* other, Vector3D direction)
{
	Vector3D supportPoint = FurthestPoint(direction) - other->FurthestPoint(direction*-1);
	return supportPoint;
}

Vector3D SphereCollider::GetClosesContactPoint(std::vector<Vector3D> collisionSimplex)
{
	//Get initiation contact on sphere
	Vector3D contact = FurthestPoint((collisionSimplex[0]- _transform->GetPosition()).normalization());
	float closesDistance = _transform->GetPosition().distance(contact);

	for (int i = 0; i < collisionSimplex.size(); i++) {

		Vector3D currentContact = FurthestPoint((collisionSimplex[i] - _transform->GetPosition()).normalization());
		float currentDistance = _transform->GetPosition().distance(collisionSimplex[i]);
		if (currentDistance < closesDistance) {
			closesDistance = currentDistance;
			contact = currentContact;
		}
		
	}
	return contact;
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
		float dot = A.dot_product(dirToOrigin);
		//checks if new line segmenet passes origin
		if (dot < 0) {
			return false;//Shapes Did not intersect;
		}

		_simplex.push_back(A);
		bool isColliding = HandleSimplex(dirToOrigin, this, other);

		if (isColliding) {

			DebugHelp().OutPutText("Sphere Collided");
			Vector3D contact =GetClosesContactPoint(_simplex);

			DebugHelp().OutPutValue("Contact X", contact.x);
			DebugHelp().OutPutValue("Contact y", contact.y);
			DebugHelp().OutPutValue("Contact z", contact.z);

			AABBReflection(contact);

			return true;
		}
		else {
			return false;
		}
	}


	return false;
}



Vector3D SphereCollider::FurthestPoint(Vector3D dir)
{
	//calculate the farthest point from the orign in the direction of the other collider in collision
	Vector3D furtuestPoint = _transform->GetPosition() + dir * _radius;
	return furtuestPoint;
}


bool SphereCollider::CollisionCheck(Collider* other)
{
	Vector3D dirToOrigin = Vector3D();
	switch (other->GetColliderType()) {
		case ColliderType::Sphere:
			 dirToOrigin = (other->GetTransform()->GetPosition() - _transform->GetPosition()).normalization();

			return GJKIntersection(other, dirToOrigin);
	
		case ColliderType::AABB:
			 dirToOrigin = (other->GetTransform()->GetPosition() - _transform->GetPosition()).normalization();

			return GJKIntersection(other, dirToOrigin);
		
	
	}
	

}

bool SphereCollider::SphereOnSphereCollision(SphereCollider* other)
{
	float distance = other->GetTransform()->GetPosition().distance(_transform->_position);
	float sum = other->GetRadius() + _radius;
	return distance <= sum;
	
}


