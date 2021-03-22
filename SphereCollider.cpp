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

bool SphereCollider::CollisionCheck(Collider* other)
{

		switch (other->GetColliderType()) {
			case ColliderType::Sphere:
			return SphereOnSphereCollision((SphereCollider*)other);
	
			case ColliderType::AABB:
			return	AABBvsSphereCollision(this, (AABoxCollider*)other);
		

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


