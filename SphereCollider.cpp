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
	//Calculate Normal to angle of insidence
	Vector3D vel  = GetOwner()->GetRigidBody()->GetCurrentVelocity();

	Vector3D normal = vel.cross_product(other->GetTransform()->GetRotation());
	if (normal.magnitude() == 0) {
		GetOwner()->GetRigidBody()->SetCurrentAcceleration(GetOwner()->GetRigidBody()->GetCurrentAcceleration() * -1.0f);
		GetOwner()->GetRigidBody()->SetCurrentVelocity(vel*-1.0f);
		return;
	}

	Vector3D normalised = normal.normalization();

	if (normalised.x != 0) {
		Vector3D newDir = Vector3D(vel.x* normalised.x, vel.y, vel.z);
		Vector3D accDir = GetOwner()->GetRigidBody()->GetCurrentAcceleration();
		GetOwner()->GetRigidBody()->SetCurrentAcceleration(Vector3D(accDir.x *normalised.x, accDir.y, accDir.z ));
	}
	else if (normalised.y != 0) {
		Vector3D newDir = Vector3D(vel.x , vel.y * normalised.y, vel.z);
		GetOwner()->GetRigidBody()->SetCurrentAcceleration(GetOwner()->GetRigidBody()->GetCurrentAcceleration().cross_product(normalised));
		Vector3D accDir = GetOwner()->GetRigidBody()->GetCurrentAcceleration();
		GetOwner()->GetRigidBody()->SetCurrentAcceleration(Vector3D(accDir.x , accDir.y* normalised.y, accDir.z));
	}
	else if (normalised.z != 0) {
		Vector3D newDir = Vector3D(vel.x, vel.y, vel.z *normalised.z);
		GetOwner()->GetRigidBody()->SetCurrentAcceleration(GetOwner()->GetRigidBody()->GetCurrentAcceleration().cross_product(normalised));
		Vector3D accDir = GetOwner()->GetRigidBody()->GetCurrentAcceleration();
		GetOwner()->GetRigidBody()->SetCurrentAcceleration(Vector3D(accDir.x , accDir.y, accDir.z*normalised.z));
	}


	
}

bool SphereCollider::CollisionCheck(Collider* other)
{

		switch (other->GetColliderType()) {
			case ColliderType::Sphere:
			return SphereOnSphereCollision((SphereCollider*)other);
	
			case ColliderType::AABB:
				if (AABBvsSphereCollision(this, (AABoxCollider*)other)) {
					AABBReflection(other);
					return true;
				}
		

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


