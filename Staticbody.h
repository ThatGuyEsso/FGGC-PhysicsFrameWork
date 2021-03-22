#pragma once
#include "RigidBody.h"

class StaticBody : public RigidBody {


public:
	StaticBody();
	StaticBody(Transform* transform);
	void Update(float t);


};