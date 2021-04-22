#include "SphereCollider.h"
#include "GameObject.h"
#include "AABoxCollider.h"
#include <algorithm>
#include "ContactResolver.h"
SphereCollider::SphereCollider():Collider::Collider(), Collision::Collision() {

	_radius = 1.0f;
	_colliderType = ColliderType::Sphere;
	Component::_type = ComponentType::Collider;
	_resititution = 0.8f;
}

SphereCollider::SphereCollider(Transform* transform, float radius): Collider::Collider(transform), Collision::Collision() {

	_radius = radius;
	_colliderType = ColliderType::Sphere;
	Component::_type = ComponentType::Collider;
	_resititution = 0.8f;
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

void SphereCollider:: Reflection(CollisionData* contactData)
{
	if (contactData->totalContacts <= 0) return;
	RigidBody* rb = GetOwner()->GetComponent<RigidBody>();

	if (rb) {

		
		//Calculate Normal to angle of insidence
		Vector3D velUnit  = rb->GetCurrentVelocity().normalization();
		//Currently Use objects position but in future use actual contact point

	

		Vector3D normal = contactData->contacts->_contactNormal;


		Vector3D reflection = velUnit - normal.normalization()* (velUnit.dot_product(normal))*2;

	
		rb->SetCurrentVelocity(reflection* rb->GetCurrentVelocity().magnitude());
		rb->SetCurrentAcceleration(reflection* rb->GetCurrentAcceleration().magnitude());


	}


	
}

Vector3D SphereCollider::Support(Collider* other, Vector3D direction)
{
	Vector3D supportPoint = FurthestPoint(direction) - other->FurthestPoint(direction*-1);
	return supportPoint;
}

void SphereCollider::DrawGUI()
{
	if (ImGui::BeginChild("Sphere Collider")) {
		//Allow users to set position of object from gui 
		float radius = _radius;
		ImGui::InputFloat("Radius", &radius);
		SetRadius(radius);
		float resitution = _resititution;
		_resititution= ImGui::InputFloat("Resititution", &resitution);

	}
	ImGui::EndChild();
}



bool SphereCollider::GJKIntersection(Collider* other, Vector3D initAxis)
{
	//Create new Simplex
	_simplex.clear();

	//get first arbitary point
	Vector3D initSimplex = Support(other, initAxis);
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
		return  HandleSimplex(dirToOrigin, this, other);

		
	}


	return false;
}

void SphereCollider::GenerateContacts(Collider* currtCollider, Collider* otherCollider, CollisionData* contactData)
{
}



Vector3D SphereCollider::FurthestPoint(Vector3D dir)
{
	//calculate the farthest point from the orign in the direction of the other collider in collision
	Vector3D pos = _transform->GetPosition();
	Vector3D furthestPoint = pos + dir * _radius;
	return furthestPoint;
}

void SphereCollider::UpdateComponent(float deltaTime)
{
}


bool SphereCollider::CollisionCheck(Collider* other)
{
	bool isColliding;
	Vector3D dirToOther = Vector3D();
	switch (other->GetColliderType()) {
		case ColliderType::Sphere:
	//	 dirToOrigin = (other->GetTransform()->GetPosition() - _transform->GetPosition()).normalization();

	///*		return GJKIntersection(other, dirToOrigin);*/
			

			isColliding= SphereOnSphereCollision((SphereCollider*)other);
			if (isColliding) {
				DebugHelp().OutPutText("S vS col");
				CollisionData* colliisonData = FindContactsInIntersection(this, other);
				if (other->GetOwner()) {
					RigidBody* rb = other->GetOwner()->GetComponent<RigidBody>();
					if (rb) {
						if (rb->GetBodyMode() == RigidBody::BodyMode::Static) {
							RigidBody* ownerRB = Component::_owner->GetComponent<RigidBody>();
							ContactResolver* resolver = new ContactResolver(ownerRB, nullptr, _resititution, colliisonData->contacts->_contactNormal,
							colliisonData->contacts->_contactPoint, colliisonData->contacts->penetrationDepth);
							resolver->Resolve(ownerRB->_deltaTime);
						}
						else {
							RigidBody* ownerRB = Component::_owner->GetComponent<RigidBody>();
							ContactResolver* resolver = new ContactResolver(ownerRB, rb, _resititution, colliisonData->contacts->_contactNormal, 
								colliisonData->contacts->_contactPoint, colliisonData->contacts->penetrationDepth);
							resolver->Resolve(ownerRB->_deltaTime);
						}
					}
				}

				return true;
			}
			return false;
		case ColliderType::AABB:
			dirToOther = (other->GetTransform()->GetPosition() - _transform->GetPosition()).normalization();

			isColliding = GJKIntersection(other, dirToOther);
			if (isColliding) {

				CollisionData* colliisonData = FindContactsInIntersection(this, other);
				if (other->GetOwner()) {
					RigidBody* rb = other->GetOwner()->GetComponent<RigidBody>();
					if (rb) {
						if (rb->GetBodyMode() == RigidBody::BodyMode::Static) {
							RigidBody* ownerRB = Component::_owner->GetComponent<RigidBody>();
							ContactResolver* resolver = new ContactResolver(ownerRB, rb, _resititution, colliisonData->contacts->_contactNormal,
							colliisonData->contacts->_contactPoint, colliisonData->contacts->penetrationDepth);
							resolver->Resolve(ownerRB->_deltaTime);
						}
						else {
							RigidBody* ownerRB = Component::_owner->GetComponent<RigidBody>();
							ContactResolver* resolver = new ContactResolver(ownerRB, rb, _resititution, colliisonData->contacts->_contactNormal, 
							colliisonData->contacts->_contactPoint, colliisonData->contacts->penetrationDepth);
							resolver->Resolve(ownerRB->_deltaTime);
						}
					}
				}
			
				return true;
			}
			return false;
	
	}
	

}

bool SphereCollider::SphereOnSphereCollision(SphereCollider* other)
{
	if (other) {
		float distance = other->GetTransform()->GetPosition().distance(_transform->_position);
		float sum = other->GetRadius() + _radius;

		return distance <= sum;

	}
	return false;
}


