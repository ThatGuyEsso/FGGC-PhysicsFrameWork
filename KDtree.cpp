#include "KDtree.h"

void KDtree::GenerateTree(std::vector<GameObject*> gameObjects)
{


Vector3D KDtree::GetMedianPosition(std::vector<GameObject*> gameObjects)
{
	Vector3D positionSum;
	for (GameObject* object:gameObjects) {
		positionSum += object->GetTransform()->GetPosition();
	}
	return  positionSum / gameObjects.size();

}
