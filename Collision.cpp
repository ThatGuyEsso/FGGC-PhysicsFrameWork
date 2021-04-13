#include "Collision.h"
#include "SphereCollider.h"
#include "AABoxCollider.h"
#include "Collider.h"

Collision::Collision()
{

}

bool Collision::HandleSimplex(Vector3D dir)
{
	if (!_simplex.empty()) {
		while (_simplex.size() <= 4) {

			//Simplex has 2 points look for the third
			if (_simplex.size() == 2) {
				LineCase(dir);
			}
			//Simplex has 3 points look for the 4th
			else if (_simplex.size() == 3) {

				if(!TriangleCase(dir)) return false;
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

void Collision::LineCase(Vector3D dir)
{
	//assuming A is the most recently added point on the simplex
//assumption Origin is always a vector position of (0,0,0)

	Vector3D AO = Vector3D() - _simplex[_simplex.size() - 1];
	//B is the first point in simplex
	Vector3D AB = _simplex[0] - _simplex[_simplex.size() - 1];

	/// <summary>
	/// Third point of the simplex is the perpendicular vector towards the orgin
	/// from point A
	/// </summary>

	Vector3D abPerp = AB.TripleProduct(AO);
	_simplex.push_back(abPerp);

}

bool Collision::TriangleCase(Vector3D dir)
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
		_simplex.push_back(normals[closesNormIndex]);
		return true;
	}
	else {
		return false;//normal is not in direction of origin/ So tetrahedron will next inclose it
	}
}

bool Collision::TetrahedronCase(Vector3D dir, std::vector<Vector3D> tetrahedron)
{
	if (tetrahedron.size() != 4)	return false;

	return SameSide(tetrahedron[0], tetrahedron[1], tetrahedron[2], tetrahedron[3], Vector3D()) &&
		SameSide(tetrahedron[1], tetrahedron[2], tetrahedron[3], tetrahedron[0], Vector3D()) &&
		SameSide(tetrahedron[2], tetrahedron[3], tetrahedron[0], tetrahedron[1], Vector3D()) &&
		SameSide(tetrahedron[3], tetrahedron[0], tetrahedron[1], tetrahedron[2], Vector3D());
}
//Checks if point is on the same side of the fourth vertex
bool Collision::SameSide(Vector3D v1, Vector3D v2, Vector3D v3, Vector3D v4, Vector3D point)
{
	//get the normal of the v1-v3 triangle
	Vector3D normal = (v2 - v1).normalization().cross_product((v3 - v1).normalization());

	/*float v4Dot = normal.dot_product((v4 - v1).normalization());*/
	float pointDot = normal.dot_product((point - v1).normalization());

	return (pointDot) <0;
}

