#include "Transform.h"
#include "GameObject.h"

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

void Transform::DrawGUI()
{//Display game object id
	if (ImGui::BeginChild("Transform")) {
		//Allow users to set position of object from gui 
		float pos[3] = { _position.x,_position.y,_position.z };
		ImGui::InputFloat3("Position ", pos, "%.2f");
		SetPosition(Vector3D(pos[0], pos[1], pos[2]));
		//Allow users to set rotation of object from gui 
		float rot[3] = { GetRotation().x,GetRotation().y,GetRotation().z };
		ImGui::InputFloat3("Rotation ", rot, "%.2f");
	

		//Allow users to set scale of object from gui 
		float scale[3] = { _scale.x,_scale.y,_scale.z };
		ImGui::InputFloat3("Scale ", scale, "%.2f");
		SetScale(Vector3D(scale[0], scale[1], scale[2]));
	}
	ImGui::EndChild();
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

void Transform::UpdateComponent(float deltaTime)
{
}
