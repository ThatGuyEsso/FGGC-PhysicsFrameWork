#pragma once
#include "Component.h"
#include "Transform.h"
#include <vector>
#include "Structs.h"

class Collider:public Component
{
public:
	Collider();
	Collider(Transform* transform);
	~Collider();
	//Component interface
	GameObject* GetOwner()override { return _owner; }
	void SetOwner(GameObject* newOwner) override { _owner = newOwner; };
	void UpdateComponent(float deltaTime);
	Component* GetClassType() { return this; }

	//Collisions
	std::vector<GameObject*> GetObjectsInCollision() { return _gameObjectsInCollision; }
	void AddCollisionObject(GameObject* object) { _gameObjectsInCollision.push_back(object); }
	Transform* GetTransform() { return _transform;  }
	void SetTransform(Transform* transform) {  _transform= transform; }
	ColliderType GetColliderType() { return _colliderType; }
	virtual bool CollisionCheck(Collider* other) =0;
	virtual Vector3D GetSupportingPoint(Collider* other,Vector3D collisionAxis) = 0;
	virtual Vector3D GenerateContacts(Collider* other, Vector3D collisionAxis) = 0;

protected:
	Transform* _transform;
	std::vector<GameObject*> _gameObjectsInCollision;

	ColliderType _colliderType = ColliderType::Sphere;

protected:

};