#include "Collider.h"
#include "GameObject.h"

Collider::Collider()
{
	_transform = nullptr;
}

Collider::Collider(Transform* transform)
{
	_transform = transform;
}

Collider::~Collider()
{
	_transform = nullptr;
	if (!_gameObjectsInCollision.empty()) {
		for (auto objects : _gameObjectsInCollision) {
			objects = nullptr;
		}
	}
	_gameObjectsInCollision.clear();

}

void Collider::UpdateComponent(float deltaTime)
{	
	//Needed to interface with components but has no implementation here
}
