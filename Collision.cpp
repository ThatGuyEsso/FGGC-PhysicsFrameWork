#include "Collision.h"
#include "SphereCollider.h"
#include "AABoxCollider.h"
#include "Collider.h"

Collision::Collision()
{

}

bool Collision::HandleSimplex(Vector3D dir, Collider* a, Collider* b)
{
	if (!_simplex.empty()) {
		while (_simplex.size() <= 4) {

			//Simplex has 2 points look for the third
			if (_simplex.size() == 2) {
				LineCase(dir,a,b);
			}
			//Simplex has 3 points look for the 4th
			else if (_simplex.size() == 3) {

				if(!TriangleCase(dir,a,b)) return false;
			}
			else if (_simplex.size() == 4) {
				return TetrahedronCase(dir,_simplex);
			}
			else {
				return false;
			}

		}
	}
	
	return false;
}

void Collision::LineCase(Vector3D dir, Collider* a, Collider* b)
{
	//assuming A is the most recently added point on the simplex
//assumption Origin is always a vector position of (0,0,0)

	Vector3D AO = (Vector3D() - _simplex[_simplex.size() - 1]).normalization();
	//B is the first point in simplex
	Vector3D AB = (_simplex[0] - _simplex[_simplex.size() - 1]).normalization();

	/// <summary>
	/// Third point of the simplex is the perpendicular vector towards the orgin
	/// from point A
	/// </summary>

	Vector3D abPerp = AB.TripleProduct(AO);

	Vector3D support = a->Support(b, abPerp);
	_simplex.push_back(support);

}

bool Collision::TriangleCase(Vector3D dir, Collider* a, Collider* b)
{
	std::vector<float> dots;
	std::vector<Vector3D> normals;

	for (int i = 0; i < _simplex.size(); i++) {

		if (i == 0)
		{
			Vector3D BA = (_simplex[i + 1] - _simplex[i]).normalization();
			Vector3D BC = (_simplex[i + 2] - _simplex[i]).normalization();
			Vector3D BACcross = BA.cross_product(BC).normalization();
			Vector3D BO = (Vector3D() - _simplex[i]).normalization();
			float dot = BACcross.dot_product(BO);
			normals.push_back(BACcross);
			dots.push_back(dot);
		}
		else if (i == 1)
		{
			Vector3D AB = (_simplex[i - 1] - _simplex[i]).normalization();
			Vector3D AC = (_simplex[i + 1] - _simplex[i]).normalization();
			Vector3D ABCcross = AB.cross_product(AC).normalization();
			Vector3D AO = (Vector3D() - _simplex[i]).normalization();
		
			float dot = ABCcross.dot_product(AO);
			normals.push_back(ABCcross);
			dots.push_back(dot);
		}	
		else if (i == 2) 
		{
			Vector3D CA = (_simplex[i - 1] - _simplex[i]).normalization();
			Vector3D CB = (_simplex[i - 2] - _simplex[i]).normalization();
			Vector3D CABcross = CA.cross_product(CB).normalization();
			Vector3D CO = (Vector3D() - _simplex[i]).normalization();
			float dot = CABcross.dot_product(CO);
			normals.push_back(CABcross);
			dots.push_back(dot);
		}
	}

	float currBestDot = 0; //The dot value of the direction closes to the origin
	float closesNormIndex = 0;
	for (int i = 0; i < dots.size(); i++) {

		if (dots[i] > currBestDot) {
			currBestDot = dots[i];
			closesNormIndex = i;
		}
	}

	if (currBestDot > 0) {

		Vector3D support = a->Support(b, normals[closesNormIndex]);
		_simplex.push_back(support);
		return true;
	}
	else {
		return false;//normal is not in direction of origin/ So tetrahedron will next inclose it
	}
}

bool Collision::TetrahedronCase(Vector3D dir, std::vector<Vector3D> tetrahedron)
{
	if (tetrahedron.size() != 4)	return false;

	 bool isInclosed =SameSide(tetrahedron[0], tetrahedron[1], tetrahedron[2], tetrahedron[3], Vector3D()) &&
		SameSide(tetrahedron[1], tetrahedron[2], tetrahedron[3], tetrahedron[0], Vector3D()) &&
		SameSide(tetrahedron[2], tetrahedron[3], tetrahedron[0], tetrahedron[1], Vector3D()) &&
		SameSide(tetrahedron[3], tetrahedron[0], tetrahedron[1], tetrahedron[2], Vector3D());
	 return  isInclosed;
}
//Checks if point is on the same side of the fourth vertex
bool Collision::SameSide(Vector3D v1, Vector3D v2, Vector3D v3, Vector3D v4, Vector3D point)
{
	//get the normal of the v1-v3 triangle
	Vector3D normal = (v2 - v1).normalization().cross_product((v3 - v1).normalization());

	float v4Dot = normal.dot_product((v4 - v1).normalization());
	float pointDot = normal.dot_product((point - v1).normalization());
	float dotProductProduct = v4Dot * pointDot;
	return (dotProductProduct) >0;
}

Collision::CollisionData* Collision::FindContactsInIntersection(Collider* currCollider, Collider* otherCollider)
{
	CollisionData* colData = new CollisionData();

	if (currCollider->GetColliderType() == otherCollider->GetColliderType()) {
		switch (currCollider->GetColliderType())
		{
			case ColliderType::Sphere:
				SphereVSphereIntersection((SphereCollider*)currCollider, (SphereCollider*)otherCollider, colData);
				break;
			case ColliderType::AABB:

				break;
		}
	}
	else {
		switch (currCollider->GetColliderType())
		{
			case ColliderType::Sphere:
				switch (otherCollider->GetColliderType())
				{
			
				case ColliderType::AABB:
					SphereVAABBIntersection((AABoxCollider*)otherCollider, (SphereCollider*)currCollider, colData);
					break;
				}
				break;
			case ColliderType::AABB:
				switch (otherCollider->GetColliderType())
				{
				case ColliderType::Sphere:
					SphereVAABBIntersection((AABoxCollider*)currCollider, (SphereCollider*)otherCollider, colData);
					break;
				}
		}
	}


	return colData;
}

Collision::CollisionData* Collision::SATBoxCollision(AABoxCollider* currCollider, AABoxCollider* otherCollider)
{
	Vector3D *axes=  GetBoxTestAxes(currCollider, otherCollider);
	float bestOverlap = 1.0f;

	unsigned bestCase;
	for (unsigned  i = 0; i < sizeof(axes); i++) {
		if(axes[i].square()<0.001) continue;	

		axes[i].normalization();
		float overlap = PenetrationOnAxis(currCollider, otherCollider, axes[i]);
		if (overlap < 0) return nullptr;
	
		if (overlap < bestOverlap) {
			bestOverlap = overlap;
			bestCase = i;
			
		}
		axes++;
	}
	Vector3D toCentre = otherCollider->GetTransform()->GetPosition() - currCollider->GetTransform()->GetPosition();
	Vector3D axis = axes[bestCase];
	//Face axis test
	if (bestCase <= 5) {
		//Get face in contact
		if (axis.dot_product(toCentre) > 0) axis *= -1.0f;

		


	}
	//Edge axis test
	else if (bestCase > 5) {

	}
	return new CollisionData;
}

float Collision::PenetrationOnAxis(AABoxCollider* currCollider, AABoxCollider* otherCollider,Vector3D axis)
{
	//Get the closest point in collider to axis
	float currHalfLength = currCollider->GetTransform()->GetPosition().distance(axis);
	float otherHalfLength = otherCollider->GetTransform()->GetPosition().distance(axis);

	Vector3D toCenter =  (otherCollider->GetTransform()->GetPosition())- currCollider->GetTransform()->GetPosition();

	float seperatingDistance = std::abs(toCenter.dot_product(axis));
	return currHalfLength + otherHalfLength - seperatingDistance;
}

void Collision::SphereVSphereIntersection(SphereCollider* currCollider, SphereCollider* otherCollider, Collision::CollisionData* data)
{
	Vector3D midLine =currCollider->GetTransform()->GetPosition()- (otherCollider->GetTransform()->GetPosition());
	Vector3D normal = midLine * (1.0f / midLine.magnitude());
	Contact* contact = data->contacts;
	contact->_contactNormal = normal;
	contact->_contactPoint = currCollider->GetTransform()->GetPosition() + midLine * 0.5f;
	contact->penetrationDepth = currCollider->GetRadius() + otherCollider->GetRadius() - midLine.magnitude();

	data->totalContacts++;

}

void Collision::SphereVAABBIntersection(AABoxCollider* box, SphereCollider* sphere, CollisionData* data)
{
	Vector3D closestPoint = box->ClosesPointToPoint(sphere->GetTransform()->GetPosition());

	Contact* contact = data->contacts;
	
	float distance = closestPoint.distance(sphere->GetTransform()->GetPosition());
	contact->_contactNormal = closestPoint - sphere->GetTransform()->GetPosition();
	contact->_contactNormal.normalization();
	contact->_contactPoint = closestPoint;
	contact->penetrationDepth = sphere->GetRadius() - distance;
	data->totalContacts++;
	
}

Vector3D* Collision::GetBoxTestAxes(AABoxCollider* currCollider, AABoxCollider* otherCollider)
{
	Vector3D Axes[15];

	//Face axes for current
	Axes[0] = currCollider->GetAxis(0);
	Axes[1] = currCollider->GetAxis(1);
	Axes[2] = currCollider->GetAxis(2);

	//Face axes for other
	Axes[3] = otherCollider->GetAxis(0);
	Axes[4] = otherCollider->GetAxis(1);
	Axes[5] = otherCollider->GetAxis(2);

	//Edge Edge Aes
	Axes[6] = currCollider->GetAxis(0).cross_product(otherCollider->GetAxis(0));
	Axes[7] = currCollider->GetAxis(0).cross_product(otherCollider->GetAxis(1));
	Axes[8] = currCollider->GetAxis(0).cross_product(otherCollider->GetAxis(2));
	Axes[9] = currCollider->GetAxis(1).cross_product(otherCollider->GetAxis(0));
	Axes[10] = currCollider->GetAxis(1).cross_product(otherCollider->GetAxis(1));
	Axes[11] = currCollider->GetAxis(1).cross_product(otherCollider->GetAxis(2));
	Axes[12] = currCollider->GetAxis(2).cross_product(otherCollider->GetAxis(0));
	Axes[13] = currCollider->GetAxis(2).cross_product(otherCollider->GetAxis(1));
	Axes[14] = currCollider->GetAxis(2).cross_product(otherCollider->GetAxis(2));
	return Axes;
}



