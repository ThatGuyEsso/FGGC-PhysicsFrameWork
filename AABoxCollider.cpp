#include "AABoxCollider.h"
#include "GameObject.h"
#include "SphereCollider.h"
#include <algorithm>
AABoxCollider::AABoxCollider():Collider::Collider(),Collision::Collision()
{
	_halfSize = Vector3D(1.0f, 1.0f, 1.0f);
    _colliderType = ColliderType::AABB;
	CalculateVertices();
}

AABoxCollider::AABoxCollider(Transform* transform, Vector3D size) : Collider::Collider(transform), Collision::Collision()
{
	_halfSize = size;
    _colliderType = ColliderType::AABB;
	CalculateVertices();
}

void AABoxCollider::SetHalfSize(Vector3D size)
{
	_halfSize = size; 
	CalculateVertices();
}

bool AABoxCollider::CollisionCheck(Collider* other)
{
	Vector3D dirToOrigin = Vector3D();
	switch (other->GetColliderType()) {
		case ColliderType::Sphere:
			dirToOrigin = (other->GetTransform()->GetPosition() - _transform->GetPosition()).normalization();
			CalculateVertices();//Update current vertex postions
			return GJKIntersection(other, dirToOrigin);
		case ColliderType::AABB:
			dirToOrigin = (other->GetTransform()->GetPosition() - _transform->GetPosition()).normalization();
			CalculateVertices();//Update current vertex postions
			return GJKIntersection(other, dirToOrigin);
		
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

		if (isColliding) {

			DebugHelp().OutPutText("Cube Collided");

			return true;
		}
		else {
			return false;
		}

	}


	return false;
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
	Vector3D closesPoint = point - _transform->GetPosition();

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
	CalculateVertices();

}

