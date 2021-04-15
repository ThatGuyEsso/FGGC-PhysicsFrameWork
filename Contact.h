#pragma once
#include "Vector3D.h"
class  Contact
{
public:
	
private:
	Vector3D _contactPoint; //Point of contact in world coordinates
	Vector3D _contactNormal; //Direction of contact in world coordinates
	float penetrationDepth; /* Depth of penetration at contact point.
							Should be midway between interpenetrating points if both bodies are specified*/



};

