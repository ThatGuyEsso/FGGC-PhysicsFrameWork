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

	float bestOverlap = 1.0f;
	bool hasCollied = false;
	int bestCase =-1;
	for (unsigned  i = 0; i < 15; i++) {

		Vector3D axis = GetBoxTestAxes(currCollider, otherCollider,i);
		if(axis.square()<0.001) continue;

		axis.normalization();
		if (i == 8);
		float overlap = PenetrationOnAxis(currCollider, otherCollider, axis);
		if (overlap < 0) return nullptr;
	
		if (overlap < bestOverlap) {
			bestOverlap = overlap;
			bestCase = i;
			hasCollied = true;
		}
		
	}
	if (hasCollied) {

		Vector3D toCentre = otherCollider->GetTransform()->GetPosition() - currCollider->GetTransform()->GetPosition();

		Vector3D axis;
		if (bestCase != -1)
			axis = GetBoxTestAxes(currCollider,otherCollider,bestCase);
		//Face axis test
		if (bestCase <= 5) {
			//Get face in contact
			if (axis.dot_product(toCentre) > 0) axis *= -1.0f;
			Vector3D vertex = otherCollider->GetHalfSize();
			//Needs to check if it's AABB or non AABB
			switch (otherCollider ->GetColliderType())
			{
			case ColliderType::AABB:
				Vector3D AABBnormal = Vector3D(0.0f, 1.0f, 0.0f);//AXis al gned to oritation normal is always up normlised
				if (otherCollider->GetAxis(0).dot_product(AABBnormal) < 0) vertex.x = -vertex.x;
				if (otherCollider->GetAxis(1).dot_product(AABBnormal) < 0) vertex.y = -vertex.y;
				if (otherCollider->GetAxis(2).dot_product(AABBnormal) < 0) vertex.z = -vertex.z;
				//convert vertex to box space
				vertex = otherCollider->GetTransform()->GetPosition() + vertex;

				//generate compile and return collision data
				CollisionData* data = new CollisionData();
				Contact* contact = data->contacts;
				contact->_contactNormal = axis,
				contact->penetrationDepth = bestOverlap;
				contact->_contactPoint = vertex;
				data->totalContacts++;
				return data;
			}

		}
		//Edge axis test
		else if (bestCase > 5) {
			Vector3D ptCurrEdge = currCollider->GetHalfSize();
			Vector3D ptOtherEdge = otherCollider->GetHalfSize();
			unsigned currAxisIndex = GetAxisIndexCurrent(bestCase);
			unsigned otherAxisIndex = GetAxisIndexOther(bestCase);
			//Getting Point data on current edge
			if (0 == currAxisIndex) ptCurrEdge.x = 0;
			else if (currCollider->GetAxis(0).dot_product(axis) > 0) ptCurrEdge.x = -ptCurrEdge.x;
			if (1 == currAxisIndex) ptCurrEdge.y = 0;
			else if (currCollider->GetAxis(1).dot_product(axis) > 0) ptCurrEdge.y = -ptCurrEdge.y;
			if (2 == currAxisIndex) ptCurrEdge.z = 0;
			else if (currCollider->GetAxis(2).dot_product(axis) > 0) ptCurrEdge.z = -ptCurrEdge.z;

			//Getting Point data on other box edge
			if (0 == otherAxisIndex) ptOtherEdge.x = 0;
			else if (otherCollider->GetAxis(0).dot_product(axis) > 0) ptOtherEdge.x = -ptOtherEdge.x;
			if (1 == otherAxisIndex) ptOtherEdge.y = 0;
			else if (otherCollider->GetAxis(1).dot_product(axis) > 0) ptOtherEdge.y = -ptOtherEdge.y;
			if (2 == otherAxisIndex) ptOtherEdge.z = 0;
			else if (otherCollider->GetAxis(2).dot_product(axis) > 0) ptOtherEdge.z = -ptOtherEdge.z;

			//Convert to world points on box

			ptCurrEdge = currCollider->GetTransform()->GetPosition() + ptCurrEdge;
			ptOtherEdge = otherCollider->GetTransform()->GetPosition() + ptOtherEdge;
			//generate compile and return collision data
			CollisionData* data = new CollisionData();
			Contact* contact = data->contacts;
			contact->_contactNormal = axis,
			contact->penetrationDepth = bestOverlap;
			contact->_contactPoint = SATGetContactPoint(currCollider->GetAxis(currAxisIndex),otherCollider->GetAxis(otherAxisIndex),
				ptCurrEdge, ptOtherEdge);
			data->totalContacts++;
			return data;
		}
	}
	return nullptr;
}

float Collision::PenetrationOnAxis(AABoxCollider* currCollider, AABoxCollider* otherCollider, Vector3D axis)
{
	//Get the closest point in collider to axis
	float currHalfLength = currCollider->ClosesPointToPoint(axis).distance(currCollider->GetTransform()->GetPosition());
	float otherHalfLength = otherCollider->ClosesPointToPoint(axis).distance(otherCollider->GetTransform()->GetPosition());

	Vector3D currPosition = currCollider->GetTransform()->GetPosition();
	Vector3D otherPosition = otherCollider->GetTransform()->GetPosition();
	Vector3D toCenter = currPosition - otherPosition;

	float seperatingDistance = std::abs(toCenter.dot_product(axis));
	float overlap = currHalfLength + otherHalfLength - seperatingDistance;
	if (overlap == 0) {
		bool test = true;
	}
	return overlap;
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
	
	float distance = box->GetTransform()->GetPosition().distance(sphere->GetTransform()->GetPosition());
	contact->_contactNormal = closestPoint - sphere->GetTransform()->GetPosition();
	contact->_contactNormal.normalization();
	contact->_contactPoint = closestPoint;
	contact->penetrationDepth = sphere->GetRadius() - distance*distance;
	data->totalContacts++;
	
}

Vector3D Collision::GetBoxTestAxes(AABoxCollider* currCollider, AABoxCollider* otherCollider, int index)
{
	if (index < 6) {
	
		if (index < 3) {
			return  currCollider->GetAxis(index);
		}
		if (index >= 3 && index < 6) {
			return  otherCollider->GetAxis(index - 3);
		}
	}

	else if (index >= 6 && index < 15) {
		if (index >= 6 && index < 9) {
			
			return currCollider->GetAxis(0).cross_product(otherCollider->GetAxis(index -6));

			
		}
		else if (index >= 9 && index <12) {
			
				return currCollider->GetAxis(1).cross_product(otherCollider->GetAxis(index -9));

			
		}
		else if (index >= 12 && index < 15) {
		
			return currCollider->GetAxis(2).cross_product(otherCollider->GetAxis(index -12));

			
		}
	}


	//Edge Edge Aes

	return Vector3D();
}

unsigned Collision::GetAxisIndexCurrent(unsigned bestIndex)
{
	if (bestIndex < 3) {
		return bestIndex;
	}
	else if (bestIndex > 5 && bestIndex < 9) {
		return 0;
	}
	else if (bestIndex >= 9 && bestIndex < 12) {
		return 1;
	}
	else if (bestIndex >= 12 && bestIndex < 15) {
		return 2;
	}

}

unsigned Collision::GetAxisIndexOther(unsigned bestIndex)
{
	if (bestIndex >= 3 && bestIndex<6) {
		return bestIndex-3;
	}
	else if (bestIndex >=6 && bestIndex < 9) {
		return bestIndex - 6;
	}
	else if (bestIndex >= 9 && bestIndex < 12) {
		return bestIndex - 9;
	}
	else if (bestIndex >= 12 && bestIndex < 15) {
		return bestIndex - 12;
	}
}

Vector3D Collision::SATGetContactPoint(Vector3D currAxis, Vector3D otherAxis, Vector3D ptOnCurrEdge, Vector3D ptOnOtherEdge)
{
	Vector3D pointToPoint = ptOnCurrEdge - ptOnOtherEdge;

	//How much are they in direciton of eachother

	float dotCurrPoint = currAxis.dot_product(pointToPoint);
	float dotOtherPoint = otherAxis.dot_product(pointToPoint);

	//How far along each edge is the closest point
	float currSqMagnitude = currAxis.square();
	float otherSqMagnitude = otherAxis.square();

	float edgeDots = otherAxis.dot_product(currAxis);
	float denom = currSqMagnitude * otherSqMagnitude - edgeDots * edgeDots;

	float a = (edgeDots * dotOtherPoint - otherSqMagnitude * dotCurrPoint) / denom;
	float b = (currSqMagnitude * dotOtherPoint - edgeDots * dotCurrPoint) / denom;


	//Point midway between nearestPoints
	Vector3D nearestPtOnOne = ptOnCurrEdge + currAxis * a;
	Vector3D nearestPtOnTwo = ptOnOtherEdge + otherAxis * b;
	return nearestPtOnOne * 0.5f + nearestPtOnTwo * 0.5f;

}



