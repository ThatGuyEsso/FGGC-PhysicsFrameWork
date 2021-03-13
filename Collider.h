#pragma once
#include "Component.h"
#include "Transform.h"
#include <vector>
class Collider:public Component
{
public:
	Collider();
	Collider(Transform* transform);
	~Collider();
	GameObject* GetOwner()override { return _owner; }
	void SetOwner(GameObject* newOwner) override { _owner = newOwner; };
	std::vector<GameObject*> GetObjectsInCollision() { return _gameObjectsInCollision; }
	void AddCollisionObject(GameObject* object) { _gameObjectsInCollision.push_back(object); }
	Transform* GetTransform() { return _transform;  }
protected:
	Transform* _transform;
	std::vector<GameObject*> _gameObjectsInCollision;
};
