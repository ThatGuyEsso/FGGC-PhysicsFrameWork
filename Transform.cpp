#include "Transform.h"

Transform::Transform()
{
	_position = Vector3D();
	_orientation = Quaternion();
	_scale = Vector3D(1.0f, 1.0f, 1.0f);
}

Transform::Transform(Vector3D position, Vector3D rotation, Vector3D scale)
{
	_position = position;
	SetRotation(rotation);
	_scale = scale;

}

void Transform::SetRotation(Vector3D rotation)
{
	XMMATRIX rotationMatrix = XMMatrixRotationX(rotation.x) * XMMatrixRotationY(rotation.y) * XMMatrixRotationZ(rotation.z);

	_orientation = MatrixToQuarternion(rotationMatrix);
}

void Transform::Rotate(Vector3D targetRotation,float scale)
{
	_orientation.normalise();
	_orientation.addScaledVector(targetRotation, scale);
}

XMMATRIX Transform::GetRotationMatrix()
{
	return QuarternionToMatrix(_orientation);
}

Vector3D Transform::GetRotation()
{
	XMMATRIX rotMatrix = QuarternionToMatrix(_orientation);
	XMVECTOR rotVector = XMQuaternionRotationMatrix(rotMatrix);
	XMFLOAT4 vector4;
	XMStoreFloat4(&vector4, rotVector);
	return Vector3D(vector4.x, vector4.y, vector4.z);
}

Quaternion Transform::MatrixToQuarternion(XMMATRIX matrix)
{
	XMVECTOR conversion = XMQuaternionRotationMatrix(matrix);
	XMFLOAT4 vector4;
	XMStoreFloat4(&vector4, conversion);

	return Quaternion(vector4.w,vector4.x, vector4.y, vector4.z );
}

XMMATRIX Transform::QuarternionToMatrix(Quaternion quarternion)
{
	{
		quarternion.normalise();
		XMMATRIX orientationMatrix;
		CalculateTransformMatrixRowMajor(orientationMatrix, _position, quarternion);
		return orientationMatrix;
	}
}
