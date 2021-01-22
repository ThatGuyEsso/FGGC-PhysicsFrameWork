#include "GameObject.h"

GameObject::GameObject(string type, Appearance* apperance)
{
	_parent = nullptr;
	//Create a new instance of object transform if none is specified
	_transform = new Transform();
	_appearance = apperance;
	_particleModel = ParticleModel(_transform);

}

GameObject::GameObject(string type, Appearance* apperance, Transform* transform)
{
	_parent = nullptr;
	//Initialise object with specific transform
	_transform = transform;
	_appearance = apperance;
	_particleModel = ParticleModel(_transform);
}

GameObject::~GameObject()
{
	_transform = nullptr;
}

void GameObject::Update(float t)
{
	// Calculate world matrix
	XMMATRIX scale = XMMatrixScaling(_transform->_scale.x, _transform->_scale.y, _transform->_scale.z);
	XMMATRIX rotation = XMMatrixRotationX(_transform->_rotation.x) * XMMatrixRotationY(_transform->_rotation.y) * XMMatrixRotationZ(_transform->_rotation.z);
	XMMATRIX translation = XMMatrixTranslation(_transform->_position.x, _transform->_position.y, _transform->_position.z);

	XMStoreFloat4x4(&_world, scale * rotation * translation);
	//if (_type == "Cube") {

	//	DebugHelp().OutPutValue("I'm a Cube", _transform->_position.z);
	//}
	if (_parent != nullptr)
	{
		XMStoreFloat4x4(&_world, this->GetWorldMatrix() * _parent->GetWorldMatrix());
	}
}

void GameObject::Draw(ID3D11DeviceContext * pImmediateContext)
{
	// NOTE: We are assuming that the constant buffers and all other draw setup has already taken place

	// Set vertex and index buffers
	pImmediateContext->IASetVertexBuffers(0, 1, _appearance->GetVertexBuffer(), _appearance->GetVertexStride(), _appearance->GetVertexBufferOffset());
	pImmediateContext->IASetIndexBuffer(_appearance->GetGeomentry().indexBuffer, DXGI_FORMAT_R16_UINT, 0);

	pImmediateContext->DrawIndexed(_appearance->GetGeomentry().numberOfIndices, 0, 0);
}
