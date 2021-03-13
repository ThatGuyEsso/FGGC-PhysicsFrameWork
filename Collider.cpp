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
