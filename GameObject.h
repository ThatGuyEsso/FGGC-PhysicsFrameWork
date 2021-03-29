#pragma once


#include <string>
#include "DebugHelp.h"
#include "Graphics.h"
#include "Quaternion.h"
#include "RigidBody.h"
#include "Component.h"
using namespace DirectX;
using namespace std;



class GameObject
{
public:
	GameObject(string type, Appearance* apperance);
	GameObject(string type, Appearance* apperance,Transform* transform);
	~GameObject();

	// Setters and Getters for transform
	void SetTransform(Transform* transform) { _transform = transform; }
	Transform* GetTransform() { return _transform; }

	
	// Setters and Getters for position/rotation/scale (queries the transform)
	void SetPosition(Vector3D position) { _transform->SetPosition( position); }

	Vector3D GetPosition() const { return _transform->GetPosition(); }

	void SetScale(Vector3D scale) { _transform->SetScale(  scale); }


	Vector3D GetScale() const { return _transform->GetScale(); }

	void SetRotation(Vector3D rotation) { _transform->SetRotation(rotation); }
	Vector3D GetRotation() const { _transform->GetRotation(); }

	string GetType() const { return _type; }

	XMMATRIX GetWorldMatrix() const { return XMLoadFloat4x4(&_world); }

	Appearance* GetAppearance() { return _appearance; }

	RigidBody* GetRigidBody() { return _rigidBody; }
	void SetRigidBody(RigidBody* rb);
	void SetParent(GameObject * parent) { _parent = parent; }

	void SetCentreOfMass(Vector3D point) { _centreOfMass = point; }
	Vector3D GetCentreOfMass() { return _centreOfMass; }
	void Update(float t);
	void Draw(ID3D11DeviceContext * pImmediateContext);

	void CalculateCentreOfMass(SimpleVertex vertices[],int vertexCount);

	//Component References

	template <typename T> T* GetComponent();


	void AddComponent(Component* componentType);

private:
	std::vector<Component*> _components;
	Transform* _transform;
	Appearance* _appearance;
	RigidBody* _rigidBody;
	Graphics* _graphics;
	string _type;
	XMFLOAT4X4 _world;
	GameObject* _parent;

	Vector3D _centreOfMass;

};


