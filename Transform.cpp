#include "Transform.h"

Transform::Transform()
{
	_position = Vector3D();
	_rotation = Vector3D();
	_scale = Vector3D(1.0f, 1.0f, 1.0f);
}

Transform::Transform(Vector3D position, Vector3D rotation, Vector3D scale)
{
	_position = position;
	_rotation = rotation;
	_scale = scale;

}
