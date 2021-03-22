#include "Staticbody.h"

StaticBody::StaticBody():RigidBody::RigidBody()
{
}

StaticBody::StaticBody(Transform* transform) : RigidBody::RigidBody(transform, Vector3D(), Vector3D(), false)
{

}

void StaticBody::Update(float t)
{

}
