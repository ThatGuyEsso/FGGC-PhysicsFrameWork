#include "AABoxCollider.h"
#include "GameObject.h"
#include "Structs.h"
#include "SphereCollider.h"
#include <algorithm>
AABoxCollider::AABoxCollider():Collider::Collider(),Collision::Collision()
{
	_halfSize = Vector3D(1.0f, 1.0f, 1.0f);
    _colliderType = ColliderType::AABB;
}

AABoxCollider::AABoxCollider(Transform* transform, Vector3D size) : Collider::Collider(transform), Collision::Collision()
{
	_halfSize = size;
    _colliderType = ColliderType::AABB;
}

bool AABoxCollider::CollisionCheck(Collider* other)
{
	switch (other->GetColliderType()) {
		case ColliderType::Sphere:
			return AABBvsSphereCollision((SphereCollider*)other,this);
		case ColliderType::AABB:
			return false;
		default:
			return false;
	}

}

bool AABoxCollider::AABBvsSphereCollision(SphereCollider* sphere, AABoxCollider* AABB)
{
	float x = std::max<float>(AABB->GetMinSize().x, (std::min<float>(sphere->GetTransform()->GetPosition().x, AABB->GetMaxSize().x)));
	float y = std::max<float>(AABB->GetMinSize().y, (std::min<float>(sphere->GetTransform()->GetPosition().y, AABB->GetMaxSize().y)));
	float z = std::max<float>(AABB->GetMinSize().z, (std::min<float>(sphere->GetTransform()->GetPosition().z, AABB->GetMaxSize().z)));

	float distance = std::sqrt((x - sphere->GetTransform()->GetPosition().x) *(x - sphere->GetTransform()->GetPosition().x) +
								(y - sphere->GetTransform()->GetPosition().y) *(y - sphere->GetTransform()->GetPosition().y) +
								(z - sphere->GetTransform()->GetPosition().z) *(z - sphere->GetTransform()->GetPosition().z));

	return distance < sphere->GetRadius();
}

bool AABoxCollider::AABoxOnAABoxCollision(AABoxCollider* other)
{
	
	return false;
	
}

Vector3D AABoxCollider::GetMaxSize()
{
	Vector3D maxVal = _transform->GetPosition() + _halfSize;
	return maxVal;
}

Vector3D AABoxCollider::GetMinSize()
{
	Vector3D minVal = _transform->GetPosition() - _halfSize;
	return minVal;
}

void AABoxCollider::AABBReflection(Collider* other)
{
}

Vector3D AABoxCollider::GetSupportingPoint(Collider* other, Vector3D collisionAxis)
{
	//Get distance to point
	float distance =_transform->GetPosition().distance(other->GetTransform()->GetPosition());
	//Project point from origin to fartherst point
	Vector3D projectedPoint = _transform->GetPosition()+collisionAxis * distance;

	//Clamp point within boundaries
	Vector3D supportPoint = Vector3D();

	supportPoint.x = std::max<float>(GetMinSize().x, std::min<float>(projectedPoint.x, GetMaxSize().x));
	supportPoint.y = std::max<float>(GetMinSize().y, std::min<float>(projectedPoint.y, GetMaxSize().y));
	supportPoint.z = std::max<float>(GetMinSize().z, std::min<float>(projectedPoint.z, GetMaxSize().z));

	return supportPoint;
}

Vector3D AABoxCollider::GenerateContacts(Collider* other, Vector3D collisionAxis)
{
	return Vector3D();
}

void AABoxCollider::DynamicResize()
{
	Geometry geometry = _owner->GetAppearance()->GetGeomentry();
	geometry.numberOfVertices;

	SimpleVertex* vertices = geometry.vertices;
	float x =0.0f, y=0.0f, z=0.0f;
	for (int i = 0; i < geometry.numberOfVertices; i++) {

		//If not null
		if (vertices) {
			//get the largest abosolute value
			if ((abs(vertices->Pos.x)>= x)) {
				x = abs(vertices->Pos.x);
			}
			if ((abs(vertices->Pos.y) >= y)) {
				y = abs(vertices->Pos.y);
			}

			if ((abs(vertices->Pos.z) >= z)) {
				z = abs(vertices->Pos.z);
			}

		}

		vertices++;
	}

	SetHalfSize(Vector3D(x, y, z));


}

