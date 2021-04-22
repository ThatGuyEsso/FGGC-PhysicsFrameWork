#include "AABoxCollider.h"
#include "GameObject.h"
#include "SphereCollider.h"
#include <algorithm>
#include "ContactResolver.h"
AABoxCollider::AABoxCollider():Collider::Collider(),Collision::Collision()
{
	_halfSize = Vector3D(1.0f, 1.0f, 1.0f);
    _colliderType = ColliderType::AABB;
	CalculateVertices();
	Component::_type = ComponentType::Collider;
	_resititution = 0.8f;
}

AABoxCollider::AABoxCollider(Transform* transform, Vector3D size) : Collider::Collider(transform), Collision::Collision()
{
	_halfSize = size;
    _colliderType = ColliderType::AABB;
	CalculateVertices();
	Component::_type = ComponentType::Collider;
}

void AABoxCollider::SetHalfSize(Vector3D size)
{
	_halfSize = size; 
	CalculateVertices();
	Component::_type = ComponentType::Collider;
}

bool AABoxCollider::CollisionCheck(Collider* other)
{
	Vector3D dirToOrigin = Vector3D();
	switch (other->GetColliderType()) {
		case ColliderType::Sphere:
			dirToOrigin = (other->GetTransform()->GetPosition() - _transform->GetPosition()).normalization();
			CalculateVertices();//Update current vertex postions

			if (GJKIntersection(other, dirToOrigin))
			{
				CollisionData* data = FindContactsInIntersection(this, other);
				if (other->GetOwner()) {
					RigidBody* rb = other->GetOwner()->GetComponent<RigidBody>();
					if (rb) {
						if (rb->GetBodyMode() == RigidBody::BodyMode::Static) {
							RigidBody* ownerRB = Component::_owner->GetComponent<RigidBody>();
							ContactResolver* resolver = new ContactResolver(ownerRB, nullptr, _resititution, data->contacts->_contactNormal,
								data->contacts->_contactPoint, data->contacts->penetrationDepth);
							resolver->Resolve(ownerRB->_deltaTime);
						}
						else {
							RigidBody* ownerRB = Component::_owner->GetComponent<RigidBody>();
							ContactResolver* resolver = new ContactResolver(ownerRB, rb, _resititution, data->contacts->_contactNormal,
								data->contacts->_contactPoint, data->contacts->penetrationDepth);
							resolver->Resolve(ownerRB->_deltaTime);
						}
					}
				}
				return true;
			}
			return false;
		case ColliderType::AABB:
			CollisionData* data = SATBoxCollision(this, (AABoxCollider*)other);
			if (data!=nullptr) {
				bool isCollided = data->totalContacts > 0;
				if (isCollided) {

					if (other->GetOwner()) {
						RigidBody* rb = other->GetOwner()->GetComponent<RigidBody>();
						if (rb) {
							if (rb->GetBodyMode() == RigidBody::BodyMode::Static) {
								RigidBody* ownerRB = Component::_owner->GetComponent<RigidBody>();
								ContactResolver* resolver = new ContactResolver(ownerRB, rb, _resititution, data->contacts->_contactNormal,
									data->contacts->_contactPoint, data->contacts->penetrationDepth);
								resolver->Resolve(ownerRB->_deltaTime);
							}
							else {
								RigidBody* ownerRB = Component::_owner->GetComponent<RigidBody>();
								ContactResolver* resolver = new ContactResolver(ownerRB, rb, _resititution, data->contacts->_contactNormal,
								data->contacts->_contactPoint, data->contacts->penetrationDepth);
								resolver->Resolve(ownerRB->_deltaTime);
							}
						}
					}
				}
				else {
					return false;
				}
			}
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

void AABoxCollider::Reflection(CollisionData* contactData)
{
}

Vector3D AABoxCollider::Support(Collider* other, Vector3D direction)
{
	Vector3D supportPoint = FurthestPoint(direction) - other->FurthestPoint(direction * -1);
	return supportPoint;
}

void AABoxCollider::GenerateContacts(Collider* currtCollider, Collider* otherCollider, CollisionData* contactData)
{
}

bool AABoxCollider::GJKIntersection(Collider* other, Vector3D initAxis)
{
	//Create new Simplex
	_simplex.clear();

	//get first arbitary point
	Vector3D initSimplex = Support(other, initAxis).normalization();
	//add startubg point to simplex
	_simplex.push_back(initSimplex);
	//DebugHelp().OutPutText("GJK Getting initial support point");
	Vector3D dirToOrigin = (Vector3D() - _simplex[0]).normalization();


	while (true) {
		//Second simplex vertex
		Vector3D A = Support(other, dirToOrigin);

		//checks if new line segmenet passes origin
		if (A.dot_product(dirToOrigin) < 0) {
			return false;//Shapes Did not intersect;
		}

		_simplex.push_back(A);
		bool isColliding= HandleSimplex(dirToOrigin,this,other);
		//DebugHelp().OutPutText("Get second support point");
		if (isColliding) {

			

			return true;
		}
		else {
			return false;
		}

	}


	return false;
}

Vector3D AABoxCollider::GetAxis(int axis)
{
	//Axis bound box so the axis are the axes of the world
	switch (axis)
	{
	case 0:
		
		return Vector3D(1, 0, 0);
	case 1:
		return Vector3D(0, 1, 0);

	
	case 2:
		return Vector3D(0, 0, 1);

	default :
		DebugHelp().OutPutText("Invalid axis");
		return Vector3D();
	}
}

void AABoxCollider::DrawGUI()
{
	ImGui::SetNextWindowSize(ImVec2(500.0f, 100.0f));
	if (ImGui::Begin("AA Box Collider")) {
		//Allow users to set position of object from gui 
		float halfsize[3] = { _halfSize.x,_halfSize.y,_halfSize.z };
		ImGui::InputFloat3("Half Size ", halfsize, "%.2f");
		SetHalfSize(Vector3D(halfsize[0], halfsize[1], halfsize[2]));
		float resitution = _resititution;
		_resititution = ImGui::InputFloat("Resititution", &resitution);


		
	}
	ImGui::End();
}

void AABoxCollider::UpdateComponent(float deltaTime)
{
}



void AABoxCollider::CalculateVertices()
{
	if (!_vertices.empty()) _vertices.clear();
	//X max
	_vertices.push_back(GetMaxSize());
	Vector3D currentVertex = Vector3D(GetMaxSize().x, GetMaxSize().y, GetMaxSize().z - _halfSize.z*2);
	_vertices.push_back(currentVertex);
	currentVertex = Vector3D(GetMaxSize().x, GetMaxSize().y- _halfSize.y* 2, GetMaxSize().z - _halfSize.z*2);
	_vertices.push_back(currentVertex);
	currentVertex = Vector3D(GetMaxSize().x, GetMaxSize().y- _halfSize.y* 2, GetMaxSize().z);
	_vertices.push_back(currentVertex);

	//X min
	_vertices.push_back(GetMinSize());
	currentVertex = Vector3D(GetMinSize().x, GetMinSize().y, GetMinSize().z + _halfSize.z * 2);
	_vertices.push_back(currentVertex);
	currentVertex = Vector3D(GetMinSize().x, GetMinSize().y + _halfSize.y * 2, GetMinSize().z + _halfSize.z * 2);
	_vertices.push_back(currentVertex);
	currentVertex = Vector3D(GetMinSize().x, GetMinSize().y + _halfSize.y * 2, GetMinSize().z);
	_vertices.push_back(currentVertex);
}

Vector3D AABoxCollider::FurthestPoint(Vector3D dir)
{

	float highestDot = 0;
	int vertexIndex =0;
	for (int i = 0; i < _vertices.size(); i++) {

		float currDot = dir.dot_product(_vertices[i]);
		if (currDot > highestDot) {
			highestDot = currDot;
			vertexIndex = i;
		}
	}

	return _vertices[vertexIndex];
}

Vector3D AABoxCollider::ClosesPointToPoint(Vector3D point)
{
	//Get Vector to point
	Vector3D closesPoint = point- _transform->GetPosition();

	//Clamp vector to bounds of cube

	if (closesPoint.x > GetMaxSize().x) closesPoint.x = GetMaxSize().x;
	if (closesPoint.x < GetMinSize().x) closesPoint.x = GetMinSize().x;

	if (closesPoint.y > GetMaxSize().y) closesPoint.y = GetMaxSize().y;
	if (closesPoint.y < GetMinSize().y) closesPoint.y = GetMinSize().y;

	if (closesPoint.z > GetMaxSize().z) closesPoint.z = GetMaxSize().z;
	if (closesPoint.z < GetMinSize().z) closesPoint.z = GetMinSize().z;
	return closesPoint;
}


void AABoxCollider::DynamicResize()
{
	Appearance* app = Component::_owner->GetComponent<Appearance>();
	if (app) {
		Geometry geometry = app->GetGeomentry();
		geometry.numberOfVertices;

		SimpleVertex* vertices = geometry.vertices;
		float x = 0.0f, y = 0.0f, z = 0.0f;
		for (int i = 0; i < geometry.numberOfVertices; i++) {

			//If not null
			if (vertices) {
				//get the largest abosolute value
				if ((abs(vertices->Pos.x) >= x)) {
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
		CalculateVertices();

	}

}

