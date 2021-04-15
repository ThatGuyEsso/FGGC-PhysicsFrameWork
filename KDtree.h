#pragma once
#include "Vector3D.h"
#include <vector>
#include "GameObject.h"
class KDtree {
	
public:

	void GenerateTree(std::vector<GameObject*> gameObject);

	struct KDnode {
	
		Vector3D point; //Point in space
		KDnode* right, *left;
		bool isLeaf = false;
		std::vector<GameObject*> gameObjectsInSpace;
	};


private:
	KDnode* rootNode;
	int maxDepth =6;
	int minObjectsInSpace = 1;

private:
	Vector3D GetMedianPosition(std::vector<GameObject*> gameObjects);
};