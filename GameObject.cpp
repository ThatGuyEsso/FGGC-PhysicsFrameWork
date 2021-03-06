#include "GameObject.h"

GameObject::GameObject(string type, Appearance* apperance)
{
	_parent = nullptr;
	//Create a new instance of object transform if none is specified
	_transform = new Transform();
	_appearance = apperance;
	_particleModel = new  ParticleModel(_transform, Vector3D(), Vector3D(), false);
	_type = type;
	_graphics = new Graphics(_appearance);
	

}

GameObject::GameObject(string type, Appearance* apperance, Transform* transform)
{
	_parent = nullptr;
	//Initialise object with specific transform
	_transform = transform;
	_appearance = apperance;
	_particleModel = new ParticleModel(_transform, Vector3D(), Vector3D(),false);
	_type = type;
	_graphics = new Graphics(_appearance);
}

GameObject::~GameObject()
{
	_transform = nullptr;
	_appearance = nullptr;
}

void GameObject::Update(float t)
{

	// Calculate world matrix
	XMMATRIX scale = XMMatrixScaling(_transform->_scale.x, _transform->_scale.y, _transform->_scale.z);
	XMMATRIX rotation = XMMatrixRotationX(_transform->_rotation.x) * XMMatrixRotationY(_transform->_rotation.y) * XMMatrixRotationZ(_transform->_rotation.z);
	XMMATRIX translation = XMMatrixTranslation(_transform->_position.x, _transform->_position.y, _transform->_position.z);
	_orientation = MatrixToQuarternion(rotation);
	_orientation.normalise();
	CalculateRotationMatrix(_orientation);

	XMStoreFloat4x4(&_world, scale * rotation * translation);
	//if (_type == "Cube") {

	//	DebugHelp().OutPutValue("I'm a Cube", _transform->_position.z);
	//}
	if (_parent != nullptr)
	{
		XMStoreFloat4x4(&_world, this->GetWorldMatrix() * _parent->GetWorldMatrix());
	}
	_particleModel->Update(t);

}

void GameObject::Draw(ID3D11DeviceContext * pImmediateContext)
{
	//call graphics component
	_graphics->Draw(pImmediateContext);
}

Quaternion GameObject::MatrixToQuarternion(XMMATRIX matrix)
{
	XMVECTOR conversion = XMQuaternionRotationMatrix(matrix);
	XMFLOAT4 vector4;
	XMStoreFloat4(&vector4, conversion);

	return Quaternion(vector4.x, vector4.y, vector4.z, vector4.w);
}

void GameObject::CalculateRotationMatrix(Quaternion orientation)
{
	
	CalculateTransformMatrixRowMajor(_orientationMatrix, _transform->GetPosition(), orientation);
}
