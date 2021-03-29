#include "GameObject.h"

GameObject::GameObject(string type, Appearance* apperance)
{
	_parent = nullptr;
	//Create a new instance of object transform if none is specified
	_transform = new Transform();
	_appearance = apperance;
	_appearance->SetOwner(this);
	_components.push_back(_appearance);
	_type = type;
	_graphics = new Graphics(_appearance);
	_centreOfMass = Vector3D();

}

GameObject::GameObject(string type, Appearance* apperance, Transform* transform)
{
	_parent = nullptr;
	//Initialise object with specific transform
	_transform = transform;
	_appearance = apperance;
	_appearance->SetOwner(this);
	_components.push_back(_appearance);
	_type = type;
	_graphics = new Graphics(_appearance);
	_centreOfMass = Vector3D();

}

GameObject::~GameObject()
{
	_transform = nullptr;
	_appearance = nullptr;
}

void GameObject::SetRigidBody(RigidBody* rb)
{
	_rigidBody = rb;
	rb->SetOwner(this);
}

void GameObject::Update(float t)
{

	// Calculate world matrix
	XMMATRIX scale = XMMatrixScaling(_transform->_scale.x, _transform->_scale.y, _transform->_scale.z);
	XMMATRIX rotation = _transform->GetRotationMatrix();
	XMMATRIX translation = XMMatrixTranslation(_transform->_position.x, _transform->_position.y, _transform->_position.z);

	XMStoreFloat4x4(&_world, scale * rotation * translation);
	//if (_type == "Cube") {

	//	DebugHelp().OutPutValue("I'm a Cube", _transform->_position.z);
	//}
	if (_parent != nullptr)
	{
		XMStoreFloat4x4(&_world, this->GetWorldMatrix() * _parent->GetWorldMatrix());
	}
	
	for (Component* comp : _components) {
		comp->UpdateComponent(t);
	}

	_rigidBody->Update(t);

}



void GameObject::Draw(ID3D11DeviceContext * pImmediateContext)
{
	//call graphics component
	_graphics->Draw(pImmediateContext);
}

void GameObject::CalculateCentreOfMass(SimpleVertex vertices[], int vertexCount)
{
	Vector3D sum = Vector3D();
	for (int i = 0; i < vertexCount; i++) {

		Vector3D converstion = Vector3D(vertices[i].Pos.x, vertices[i].Pos.y, vertices[i].Pos.z);

		sum += converstion;
	}

	_centreOfMass = sum / vertexCount;
	DebugHelp().OutPutValue("Centre Of Mass", _centreOfMass);

}



template<typename T>
T* GameObject::GetComponent()
{
	T* compClass = new T();
	for (Component* comp : _components) {
		//Cast component to target component
		comp = dynamic_cast<T*>(compClass);
		
		//if not false
		if (comp) {
			return comp;
		}
	}

	//if cast failed or no component found return null
	return nullptr;
}

void GameObject::AddComponent(Component* newComp)
{
	for (Component* comp : _components) {

		//If game object already has a collider of said type
		if (comp->GetComponentType() == newComp->GetComponentType()) {
			comp = newComp;
			return;
		}
	}

	_components.push_back(newComp);
}
