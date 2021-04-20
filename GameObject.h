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
	GameObject(string type);
	GameObject(string type,Transform* transform);
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

	void SetParent(GameObject * parent) { _parent = parent; }

	void SetCentreOfMass(Vector3D point) { _centreOfMass = point; }
	Vector3D GetCentreOfMass();
	void Update(float t);


	void CalculateCentreOfMass(SimpleVertex vertices[],int vertexCount);

	//Component References

	template<typename T>
	T* GetComponent()
	{
		T* ptr;
		for (Component* comp : _components) {
			//Cast component to target component
			ptr = dynamic_cast<T*>(comp);

			//if not false
			if (ptr) {
				return ptr;
			}
		}

		//if cast failed or no component found return null
		return nullptr;
	}

	template<typename T>
	void RemoveComponent()
	{
		T* ptr;
		int compIndex;
		bool compExist = false;
		for (int i = 0; i < _components.size();i++) {
			//Cast component to target component
			ptr = dynamic_cast<T*>(_components[i]);

			//if not false
			if (ptr) {
				compIndex = i;
				compExist = true;
				break;
			}
		}
		if (compExist) {
			std::vector<Component*> copyList = _components;
			_components.clear();

			for (int i = 0; i < copyList.size(); i++) {
				if (i != compIndex)_components.push_back(copyList[i]);
			}
		}
	

	}

	void AddComponent(Component* componentType);

private:
	std::vector<Component*> _components;
	Transform* _transform;
	string _type;
	XMFLOAT4X4 _world;
	GameObject* _parent;
	Vector3D _centreOfMass;

};


